#pragma once

#include "Detector.hpp"

class CascadeDetector: public Detector
{
	std::shared_ptr<cv::CascadeClassifier> m_cascade;
	double m_scaleFactor = 1.1;
	int m_minNeighbours = 3;

public:

	CascadeDetector(const std::string &xml);
	~CascadeDetector();

	void detect(const CVUtils::Image &input);

	double getScaleFactor() const;
	int getMinNeighbours() const;
	void setScaleFactor(double scaleFactor);
	void setMinNeighbours(int minNeighbours);
};