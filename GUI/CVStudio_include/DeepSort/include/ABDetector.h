#pragma once

#include "Detector.hpp"
#include "DSDetector.h"

class ABDetector : public DSDetector
{
	std::shared_ptr<Detector> m_detector;

public:

	ABDetector(const std::shared_ptr<Detector> &detector);
	~ABDetector();

	void detect(const cv::Mat &frame, std::vector<CVUtils::BBoxf> &bboxes);
};
