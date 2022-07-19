#pragma once

#include "Stream/Streamer.hpp"
#include "Detector.hpp"

#include <memory>


class Visualizer
{
	bool m_showDetLabels = true;
	bool m_showDetConfidence = true;

public:

	Visualizer();
	~Visualizer();

	void showDetections(std::shared_ptr<Detector> detector, const CVUtils::Image &img, const int delay = 0, CVUtils::Size visSize = CVUtils::Size(), const std::vector<std::string> &labelmap = std::vector<std::string>());
	void showDetections(std::shared_ptr<Detector> detector, std::shared_ptr<CVUtils::Streamer> streamer, const int delay = 1, CVUtils::Size visSize = CVUtils::Size(), const std::vector<std::string> &labelmap = std::vector<std::string>());

	void drawRect(CVUtils::Image &img, const CVUtils::BBox &rect, const cv::Scalar &color = cv::Scalar(0, 0, 255));
    void drawText(CVUtils::Image &img, const std::string &text, const CVUtils::Point &tl, const cv::Scalar &color = cv::Scalar(0, 255, 0));

	void drawDetection(CVUtils::Image &img, const Detection &det, const std::vector<std::string> &labelmap = std::vector<std::string>());

	void setShowDetLabels(bool showDetLabels);
	void setShowDetConfidence(bool showDetConfidence);

	bool getShowDetLabels() const;
	bool getShowDetConfidence() const;
};
