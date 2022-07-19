#pragma once

#include "Detector.hpp"
#include "OpenCVNetManager.hpp"

class OpenCVSSD: public Detector
{
	std::shared_ptr<OpenCVNetManager> m_netManager;
	float m_threshold = 0.5f;

public:

	OpenCVSSD();
	~OpenCVSSD();

	void resetFromCaffe(const std::string &prototxt, const std::string &caffemodel);

	void setThreshold(const float threshold);
	float getThreshold() const;

	void resizeInput(const CVUtils::Size &newSize);
	void setNumChannels(int newChannels);

	void reshape(const CVUtils::Shape3D &newShape);

	void detect(const CVUtils::Image &input) override;

	CVUtils::Size inputSize() const;
	CVUtils::Shape3D inputShape3D() const;
};

