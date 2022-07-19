#pragma once
#include "SingleImageAlgorithm.hpp"
#include "Bbox.hpp"

struct Detection
{
	int label;
	float confidence;
	CVUtils::BBox bbox;

	bool operator == (const Detection &other)
	{
		return label == other.label && confidence == other.confidence && bbox == other.bbox;
	}

	float iou(const Detection &other) const
	{
		return float(bbox.iou(other.bbox));
	}
};

class Detector: public SingleImageAlgorithm
{
protected:

	std::vector<Detection> m_detections;
	
public:

	Detector(const std::string &name, const std::string &engine);
	virtual ~Detector();

	void run(const CVUtils::Image &input);

	virtual void detect(const CVUtils::Image &input) = 0;

	std::vector<Detection> getDetections() const;
};