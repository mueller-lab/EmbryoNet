#pragma once

#include "NetManager.hpp"

class OpenCVNetManager: public NetManager
{
	cv::Ptr<cv::dnn::Net> m_net;

	CVUtils::Shape3D m_shape;

	cv::Mat m_outputMat;

public:

	OpenCVNetManager();
	~OpenCVNetManager();

	void resetFromCaffe(const std::string &prototxt, const std::string &caffemodel);
	void resetFromONNX(const std::string& onnx, const int explicitChannels = 0);

	virtual void forward(const CVUtils::Image &input) override;
	virtual void forward(const std::map<std::string, cv::Mat>& inputs) override;

	float *outputData();
	const float *constOutputData();

	std::vector<float> getFeatures(const std::string& name) override;

	std::vector<int> inputShape() const;
	std::vector<int> outputShape() const;

	void resizeInput(const CVUtils::Size &newSize);
	void setNumChannels(int newChannels);

	void reshape(const CVUtils::Shape3D &newShape);

	cv::Mat outputMat() const;

};