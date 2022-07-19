#include "EmbryoExperimentRunner.h"

#include "EmbryoExperiment.h"
#include "EmbryoSource.h"
#include "ResultRepresentor.h"

#include <chrono> 
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

EmbryoExperimentRunner::EmbryoExperimentRunner(const nlohmann::json& config)
{
	this->construct(config);
}

EmbryoExperimentRunner::~EmbryoExperimentRunner()
{
}


namespace
{

	std::string getModelName(const nlohmann::json& algorithmConfig)
	{
		const std::string path_to_net = algorithmConfig["classifier"]["path_to_net"].get<std::string>();
		size_t model_name_start = path_to_net.rfind('/');
		size_t model_name_end = path_to_net.rfind('.');

		if (model_name_start == std::string::npos)
		{
			throw std::runtime_error("Please check config path_to_net field. '/' was not found in path_to_net");
		}

		if (model_name_end == std::string::npos)
		{
			throw std::runtime_error("Please check config path_to_net field. '.' was not found in path_to_net");
		}
		if (model_name_end <= model_name_start)
		{
			throw std::runtime_error("Please check config path_to_net field. Name is not correct.");
		}

		return path_to_net.substr(model_name_start, model_name_end - model_name_start);
	}

} // namespace

void EmbryoExperimentRunner::construct(const nlohmann::json& config)
{
	auto experimentConfig = config["Algorithm"];
	experimentConfig["classes"] = config["classes"];
	m_experiment.reset(new EmbryoExperiment(experimentConfig));
	m_experiment->resetClasses(config["classes"]);

	const auto& sourceConfig = config["Source"];
	m_source.reset(new EmbryoSource(sourceConfig));

	const auto& resultRepresentorConfig = config["ResultRepresenter"];
	m_resultRepresentor.reset(new ResultRepresentor(resultRepresentorConfig, getModelName(experimentConfig)));
	m_resultRepresentor->resetClasses(config["classes"]);

	if (config.count("MicroscopePattern"))
	{
		m_microscopePattern = config["MicroscopePattern"].get<std::string>();

		const auto& timeStrToFloat = [](const std::string& timePointStr) -> float
		{
			const float hh = std::stof(timePointStr.substr(0, 2));
			const float mm = std::stof(timePointStr.substr(3, 2));

			return (hh * 60.0f + mm) / (24.0f * 60.0f);
		};

		float startTime = timeStrToFloat(config["StartPoint"].get<std::string>());
		float stopTime = timeStrToFloat(config["StopPoint"].get<std::string>());

		if (stopTime >= startTime)
		{
			startTime = 0.0f;
			stopTime = 1.0f;
		}

		if (std::fabs(stopTime) < 1e-5)
		{
			stopTime = 1.0f;
		}

		m_startPoint = startTime;
		m_stopPoint = stopTime;

	}

}

std::pair <cv::Mat, std::string> EmbryoExperimentRunner::getNextImageJsonPair()
{
	return (*m_source)();
}

void EmbryoExperimentRunner::runStandAlone()
{
	while (!m_isInitialOver)
	{
		// get input image
		const auto& imagePathPair = getNextImageJsonPair();
		runStep(imagePathPair);
	}

	for (const auto& imagePath : m_matPathes)
	{
		std::pair<cv::Mat, std::string> imgPathPairSecondRun = { cv::Mat(), imagePath };
		runStep(imgPathPairSecondRun);
	}
}

std::pair<cv::Mat, std::string> EmbryoExperimentRunner::runStep(std::pair<cv::Mat, std::string> imagePathPair)
{
	if (!m_isInitialOver)
	{
		return initialStep(imagePathPair);
	}
	else
	{
		return fixStep(imagePathPair);
	}
}

float EmbryoExperimentRunner::findAge(const std::string& imagePath)
{
	float age = 0;

	bool foundRegex = false;
	std::string frameNumStr;
	float startTime = 0.0;
	float stopTime = 1.0;

	std::map<std::string, std::pair<float, float>> pattern2startpoint;

	if (m_microscopePattern.empty())
	{
		pattern2startpoint = {
			{"LO[0-9]+", {0.0f, 1.0f}}
		};

	}
	else
	{
		pattern2startpoint = {
			{m_microscopePattern, {m_startPoint, m_stopPoint}}
		};
	}

	for (const auto keyVal : pattern2startpoint)
	{
		boost::regex expr(keyVal.first);
		boost::smatch what;
		if (!boost::regex_search(imagePath, what, expr))
		{
			continue;
		}

		frameNumStr = what[0].str().substr(2);
		startTime = keyVal.second.first;
		stopTime = keyVal.second.second;
		foundRegex = true;
	}

	if (!foundRegex && m_microscopePattern.empty())
	{
		throw std::runtime_error("Could not find image microscope pattern in image name. \nPlease check settings. ");
	}

	boost::filesystem::path parentPath = boost::filesystem::path(imagePath).parent_path();
	float maxFilesCnt = float(std::count_if(
		boost::filesystem::directory_iterator(parentPath),
		boost::filesystem::directory_iterator(),
		static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file)));

	try
	{
		const float fimageNum = std::stof(frameNumStr);
		age = startTime + (fimageNum / maxFilesCnt) * (stopTime - startTime);
	}
	catch (const std::invalid_argument&)
	{
		throw std::runtime_error("Microscope pattern is incorrect. \nPlease check settings.");
	}

	return age;
}



std::pair<cv::Mat, std::string> EmbryoExperimentRunner::initialStep(std::pair<cv::Mat, std::string> imagePathPair)
{
	nlohmann::json resultJSONPerImage;
	const auto& image = imagePathPair.first;
	const auto& imagePath = imagePathPair.second;

	// if image is empty, we are done
	if (image.empty())
	{
		m_isInitialOver = true;
		m_resultRepresentor->setRun(true);
		m_fixedJsonResults = (*m_experiment)();
		return { cv::Mat(), std::string() };
	}

	m_matPathes.push_back(imagePath);


	float age = findAge(imagePath);

	const auto t1 = std::chrono::high_resolution_clock::now();
	// aply algorithm on the image
	const auto& algorithmResult = (*m_experiment)(image, age);
	const auto t2 = std::chrono::high_resolution_clock::now();


	resultJSONPerImage["detection_list"] = nlohmann::json::parse(algorithmResult);
	resultJSONPerImage["source_name"] = imagePath;

	// dump or visualize results
	(*m_resultRepresentor)(image, resultJSONPerImage.dump(4));

	return { image, resultJSONPerImage.dump(4) };
}

std::pair<cv::Mat, std::string> EmbryoExperimentRunner::fixStep(std::pair<cv::Mat, std::string> imagePathPair)
{
	cv::Mat image = cv::imread(imagePathPair.second);
	nlohmann::json resultJSONPerImage;
	resultJSONPerImage["detection_list"] = nlohmann::json::parse(m_fixedJsonResults[m_fixedJsonResultIndex]);
	resultJSONPerImage["source_name"] = imagePathPair.second;
	(*m_resultRepresentor)(image, resultJSONPerImage.dump(4));
	m_fixedJsonResultIndex++;
	return { image, resultJSONPerImage.dump(4) };
}

std::vector<std::string> EmbryoExperimentRunner::getPaths() const
{
	return m_source->getPathes();
}
