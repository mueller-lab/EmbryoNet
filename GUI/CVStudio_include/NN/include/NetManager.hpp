#pragma once

#include "Image.hpp"

class NetManager
{
public:

	enum FRAMEWORK
	{
		CAFFE, CAFFE2, OPENCV, OPENVINO
	};

	enum DATA_FORMAT
	{
		NCHW,
		NHWC
	};

protected:

	FRAMEWORK m_framework;
	DATA_FORMAT m_dataFormat;
	
public:

	NetManager(const FRAMEWORK &framework, const DATA_FORMAT &dataFormat);
	virtual ~NetManager();

	virtual void forward(const CVUtils::Image &input) = 0;
	virtual void forward(const std::map<std::string, cv::Mat>& inputs);

	std::vector<float> predict(const CVUtils::Image &input);
	std::vector<float> predict(const std::map<std::string, cv::Mat> & inputs);

	virtual float *outputData() = 0;
	virtual const float *constOutputData() = 0;

	virtual std::vector<float> getFeatures(const std::string& name) = 0;

	virtual std::vector<int> inputShape() const = 0;
	virtual std::vector<int> outputShape() const = 0;

	virtual void resizeInput(const CVUtils::Size &newSize) = 0;

	int inputNum() const;
	int inputChannels() const;
	int inputHeight() const;
	int inputWidth() const;

	int outputNum() const;
	int outputChannels() const;
	int outputHeight() const;
	int outputWidth() const;

	DATA_FORMAT dataFormat() const;
	FRAMEWORK framework() const;

	CVUtils::Size inputSize() const;
	CVUtils::Shape3D inputShape3D() const;
};

namespace std
{
	::std::string to_string(NetManager::FRAMEWORK framework);
	::std::string to_string(NetManager::DATA_FORMAT dataFormat);
}
