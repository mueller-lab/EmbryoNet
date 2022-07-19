#pragma once

#ifdef WITH_CAFFE

#include "Detector.hpp"
#include "CaffeNetManager.hpp"

class CaffeSSD: public Detector
{

	std::shared_ptr<CaffeNetManager> m_netManager;
	float m_threshold = 0.5f;

public:

	CaffeSSD(const std::string &prototxt, const std::string &caffemodel);
	~CaffeSSD();

	void setThreshold(const float threshold);
	float getThreshold();

	void resizeInput(const CVUtils::Size &newSize);
	void setDevice(const caffe::Caffe::Brew &device);

	void detect(const CVUtils::Image &input) override;
	
	CVUtils::Size inputSize() const;
	CVUtils::Shape3D inputShape3D() const;

};


#endif // WITH_CAFFE

