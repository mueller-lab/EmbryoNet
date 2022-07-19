#pragma once

#ifdef WITH_CAFFE2

#include "Detector.hpp"
#include "Caffe2Netmanager.hpp"

class Caffe2Detector: public Detector
{
	std::shared_ptr<Caffe2NetManager> m_netManager;
	float m_threshold = 0.5f;

public:

	Caffe2Detector(const std::string& architecture, const std::string& weights);
	~Caffe2Detector();

	void setThreshold(const float threshold);
	float getThreshold() const;

	void resizeInput(const CVUtils::Size &newSize);
	void reshape(const CVUtils::Shape3D &newShape);

	void detect(const CVUtils::Image &input) override;

	CVUtils::Size inputSize() const;
	CVUtils::Shape3D inputShape3D() const;

private:

	void setImInfo();


};

#endif

