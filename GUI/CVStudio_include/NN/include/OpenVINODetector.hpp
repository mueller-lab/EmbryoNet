#pragma once

#ifdef WITH_OPENVINO

#include "Detector.hpp"
#include "OpenVINONetManager.hpp"

class OpenVINODetector: public Detector
{
	std::shared_ptr<OpenVINONetManager> m_netManager;
	float m_threshold = 0.5f;

public:

	OpenVINODetector(const std::string& xml, const std::string& bin, const std::string& device);
	~OpenVINODetector();

	void setThreshold(const float threshold);
	float getThreshold() const;

	void resizeInput(const CVUtils::Size &newSize);

	void detect(const CVUtils::Image &input) override;

	CVUtils::Size inputSize() const;
	CVUtils::Shape3D inputShape3D() const;

};

#endif // WITH_OPENVINO

