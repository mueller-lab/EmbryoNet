#pragma once

#ifdef WITH_OPENVINO

#include "NetManager.hpp"
#include <inference_engine.hpp>

class OpenVINONetManager final: public NetManager
{

public:

	OpenVINONetManager(const std::string& xml, const std::string& bin, const std::string& device);
	~OpenVINONetManager();

	void forward(const CVUtils::Image &input) override;

	float *outputData() override;
	const float *constOutputData() override;

	std::vector<float> getFeatures(const std::string& name) override;

	std::vector<int> inputShape()  const override;
	std::vector<int> outputShape() const override;

	void resizeInput(const CVUtils::Size &newSize) override;

private:

	std::string m_inputName;
	std::string m_outputName;

	unsigned char* m_inputData;

	InferenceEngine::CNNNetwork m_cnnNetwork;
	InferenceEngine::InferencePlugin m_plugin;
	InferenceEngine::InferRequest m_inferRequest;

	CVUtils::Shape3D m_inputShape3D;
	std::vector<int> m_outputShape;
};


#endif // WITH_OPENVINO

