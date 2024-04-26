#include "EmbryoClassifier.h"

#include "OpencvNetManager.hpp"
#include <opencv2/opencv.hpp>
#include <codecvt>

EmbryoClassifier::EmbryoClassifier(const nlohmann::json& config)
#if WITH_ONNXRUNTIME
	:     m_env(ORT_LOGGING_LEVEL_WARNING, "test")
#endif
{

	const std::string type = config["type"];
	m_isrun = static_cast<bool>(config["run"]);

	m_outputClasses = static_cast<int>(config["outputClasses"]);

	//std::string dump_string = config.dump();
	const int width = static_cast<int>(config["width"]);

	const int height = static_cast<int>(config["height"]);
	m_size.reset(new cv::Size(width, height));


#if WITH_ONNXRUNTIME

	m_shapeLen = 4;
	const int THREADS = 1;
	if (!m_sessionOptions)
	{
		delete m_sessionOptions;
		m_sessionOptions = nullptr;
	}
	m_sessionOptions = new Ort::SessionOptions;
	m_sessionOptions->SetIntraOpNumThreads(THREADS);
	int deviceID = 0;
	OrtSessionOptionsAppendExecutionProvider_CUDA(*m_sessionOptions, deviceID);

	// ORT_DISABLE_ALL -> To disable all optimizations
	// ORT_ENABLE_BASIC -> To enable basic optimizations (Such as redundant node removals)
	// ORT_ENABLE_EXTENDED -> To enable extended optimizations (Includes level 1 + more complex optimizations like node fusions)
	// ORT_ENABLE_ALL -> To Enable All possible opitmizations
	
	m_sessionOptions->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

	const std::string modelPath = config["path_to_net"];

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	const std::wstring wmodelPath = converter.from_bytes(modelPath);



	
	m_session = new Ort::Session(m_env, wmodelPath.c_str(), *m_sessionOptions);

	
	m_memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

	
	m_memoryInfo_t_for_conv = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

	
	m_memoryInfo_t_for_fc = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

	const int numInputs = 3; //data , t_for_conv, t_for fc. For more information look in netron.  
	for (int inputIdx = 0; inputIdx < numInputs; ++inputIdx)
	{
		Ort::TypeInfo typeInfo = m_session->GetInputTypeInfo(inputIdx);
		m_tensorInfo = new Ort::Unowned<Ort::TensorTypeAndShapeInfo>(typeInfo.GetTensorTypeAndShapeInfo());
		std::vector<int64_t> inputShape = m_tensorInfo->GetShape();
		inputShape[0] = 8;
		m_inpuNodeDims.push_back(inputShape);
	}

#else

	if (type == std::string("OpenCV"))
	{
		m_netManager.reset(new OpenCVNetManager());

		const std::string pathToNet = config["path_to_net"];
		const std::string engine = config["engine"];
		if (engine == "ONNX")
		{
			OpenCVNetManager* opencvNetManager = dynamic_cast<OpenCVNetManager*>(m_netManager.get());
			if (!opencvNetManager)
			{
				throw std::runtime_error("dynamic_cast to OpenCVNetManager returned NULL");
			}
			opencvNetManager->resetFromONNX(pathToNet, 3);
		}
		else
		{
			throw std::runtime_error(std::string("Unsuported engine for OpenCVNetManager: ") + engine);
		}
	}
	else
	{
		throw std::runtime_error(std::string("Got type ") + type + std::string(" excpected OpenCV"));
	}

#endif


#if WITH_ONNXRUNTIME
#else
	m_netManager->resizeInput(*m_size);
#endif

	size_t scalarChannels = 0;
	const auto& toCVScalar = [&scalarChannels](const nlohmann::json& scalarJSON) -> cv::Scalar_<float>
	{
		std::vector<float> scalarVec;
		for (const auto& floatElem : scalarJSON)
		{
			scalarVec.push_back(static_cast<float>(floatElem));
		}

		const size_t grayScale = 1;
		const size_t threeChannel = 3;

		scalarChannels = scalarVec.size();
		if (scalarVec.size() == grayScale)
		{
			return cv::Scalar_<float>(scalarVec[0]);
		}
		else if (scalarVec.size() == threeChannel)
		{
			return cv::Scalar_<float>(scalarVec[0], scalarVec[1], scalarVec[2]);
		}
		else
		{
			throw std::runtime_error("Unsupported color format in toCVScalar");
		}

		return cv::Scalar_<float>();
	};

	const auto& minus = toCVScalar(config["minus"]);
	const auto& divide = toCVScalar(config["divide"]);

	m_minus.reset(new cv::Mat(*m_size, CV_MAKETYPE(CV_32F, scalarChannels), minus));
	m_divide.reset(new cv::Mat(*m_size, CV_MAKETYPE(CV_32F, scalarChannels), divide));
}

EmbryoClassifier::~EmbryoClassifier()
{

#if WITH_ONNXRUNTIME

#define SET_TO_NULL_IF_EXISTS(obj) \
	if (obj)                       \
	{                              \
		delete obj;                \
		obj = nullptr;             \
	}

	SET_TO_NULL_IF_EXISTS(m_session);

	SET_TO_NULL_IF_EXISTS(m_sessionOptions);

#endif
}

void EmbryoClassifier::resetClasses(const nlohmann::json& config)
{
	const auto& toEmbryoClass = [](const nlohmann::json& colorJSON) -> EmbryoClass
	{
		std::vector<int> color;

		for (const auto& floatElem : colorJSON["color"])
		{
			color.push_back(static_cast<int>(floatElem));
		}

		const std::string name = colorJSON["name"];
		EmbryoClass embryo = { colorJSON["id"], name, color };

		return embryo;
	};

	for (const auto& embryoClass : config)
	{
		this->m_embryoClasses.push_back(toEmbryoClass(embryoClass));
	}
}

std::vector<std::vector<float> > EmbryoClassifier::classify(
	const cv::Mat& frame,
	const std::vector<CVUtils::BBoxf>& boxes,
	const float& age)
{

	std::vector<std::vector<float> > classificationResult;


	for (const auto& pos : boxes)
	{
		if (m_isrun)
		{
			const std::vector<float>& xywh = pos.xywh();

			const int x = static_cast<int>(xywh[0]);
			const int y = static_cast<int>(xywh[1]);
			const int w = static_cast<int>(xywh[2]);
			const int h = static_cast<int>(xywh[3]);

			const int tlx = std::max(0, x);
			const int tly = std::max(0, y);;
			const int brx = std::min(tlx + w, frame.cols);
			const int bry = std::min(tly + h, frame.rows);

			cv::Rect cvBox(cv::Point(tlx, tly), cv::Point(brx, bry));


			cv::Mat embryoROI = frame(cvBox);


			cv::resize(embryoROI, embryoROI, *m_size);
			cv::Mat normalizedEmbryoROI;

			embryoROI.convertTo(normalizedEmbryoROI, CV_32FC3);


			normalizedEmbryoROI = normalizedEmbryoROI - (*m_minus);
			normalizedEmbryoROI = normalizedEmbryoROI / (*m_divide);

			std::vector<cv::Mat> normalizedFlippedBatch = generateFlippedImages(normalizedEmbryoROI);

			const int batchSize = normalizedFlippedBatch.size();
			
			const int sz_t_for_conv[] = { batchSize, 1, 224, 224 };
			const int sz_t_for_fc[] = { batchSize, 1 };

			cv::Mat inputBigBlob = cv::dnn::blobFromImages(normalizedFlippedBatch);

			const int numberOfDimentionsNCHW = 4;
			const int numberOfDimentionsNC = 2;
			const std::map<std::string, cv::Mat> inputs = {
				{ "data", inputBigBlob },
				{ "t_for_conv", cv::Mat::ones(numberOfDimentionsNCHW, sz_t_for_conv, CV_32FC1) * age }, 
				{ "t_for_fc", cv::Mat::ones(numberOfDimentionsNC, sz_t_for_fc, CV_32FC1) * age } 
			};

#if WITH_ONNXRUNTIME

			const size_t inputTensorSize = batchSize *
				normalizedEmbryoROI.cols *
				normalizedEmbryoROI.rows *
				normalizedEmbryoROI.channels() *
				sizeof(float);

			Ort::Value inputsArray[] = {
				Ort::Value::CreateTensor<float>(
					const_cast<OrtMemoryInfo*>(m_memoryInfo),
					reinterpret_cast<float*>(inputs.at("data").data),
					inputTensorSize,
					const_cast<const int64_t*>(m_inpuNodeDims[0].data()),
					m_shapeLen),
				Ort::Value::CreateTensor<float>(
					const_cast<OrtMemoryInfo*>(m_memoryInfo_t_for_conv),
					reinterpret_cast<float*>(inputs.at("t_for_conv").data),
					inputTensorSize / normalizedEmbryoROI.channels(),
					const_cast<const int64_t*>(m_inpuNodeDims[1].data()),
					m_shapeLen),
				Ort::Value::CreateTensor<float>(
					const_cast<OrtMemoryInfo*>(m_memoryInfo_t_for_conv),
					reinterpret_cast<float*>(inputs.at("t_for_fc").data),
					sizeof(float) * batchSize,
					const_cast<const int64_t*>(m_inpuNodeDims[2].data()),
					2)
			};

			const std::vector<const char*> inputNodeNames = { "data", "t_for_conv", "t_for_fc"};
			const std::vector<const char*> outputNodeNames = { "output" };

			int inputCount = 3; //data , t_for_conv, t_for fc. For more information look in netron. 
			int outputNamesCount = 1;
			auto outputTensors = m_session->Run(Ort::RunOptions{ nullptr },
				inputNodeNames.data(),
				inputsArray,
				inputCount,
				outputNodeNames.data(),
				outputNamesCount);

			const float* floatArr = outputTensors.front().GetTensorMutableData<float>();
			std::vector<float> currResult(floatArr, floatArr + batchSize * m_outputClasses);

#else
			std::vector<float> currResult = m_netManager->predict(inputs);
#endif

		

			std::vector<float> result(m_outputClasses, .0f);
			for (size_t batchIdx = 0; batchIdx < batchSize; ++batchIdx)
			{
				for (size_t clsIdx = 0; clsIdx < m_outputClasses; ++clsIdx)
				{
					result[clsIdx] += currResult[batchIdx * m_outputClasses + clsIdx] / batchSize;
				}
			}

			classificationResult.push_back(result);

		}
		else
		{
			std::vector<float> result;
			result.resize(m_outputClasses);
			result[m_defaultClass] = 1;
			classificationResult.push_back(result);

		}
	}
	return classificationResult;
}



std::vector<cv::Mat> EmbryoClassifier::generateFlippedImages(const cv::Mat& image)
{
	std::vector<cv::Mat> output;

	cv::Mat normal = image;
	
	cv::Mat FV;
	cv::flip(image, FV, 0);
	cv::Mat FH;
	cv::flip(image, FH, 1);
	cv::Mat flippedAll;
	cv::flip(image, flippedAll, -1);

	cv::Mat RR;
	cv::rotate(image, RR, cv::ROTATE_90_CLOCKWISE);
	cv::Mat RL;
	cv::rotate(image, RL, cv::ROTATE_90_COUNTERCLOCKWISE);

	cv::Mat RL_FH;
	cv::flip(RL, RL_FH, 1);

	cv::Mat FH_RL;
	cv::rotate(FH, FH_RL, cv::ROTATE_90_COUNTERCLOCKWISE);

	output.push_back(normal);
	output.push_back(FV);
	output.push_back(FH);
	output.push_back(flippedAll);
	output.push_back(RR);
	output.push_back(RL);
	output.push_back(RL_FH);
	output.push_back(FH_RL);

	//output = { normal };
	return output;
}


void EmbryoClassifier::setDefaultClass(const int defalutClass)
{
	m_defaultClass = defalutClass;
}

std::string EmbryoClassifier::getDefaultClassName() const
{
	return m_default_class_name;
}
