#pragma once

#include "Bbox.hpp"

//
// DSDetector abstract interface
//
// DSDetector is an algorithm, which generates a vector of object-class bounding boxes
// given an input image. Those detections are matched to existing tracks
// via Macther algorithm during frame processing step of DeepSort algorithm.
//
// If DeepSort is given an Encoder, then it uses the DSDetector's bounding boxes
// to generate features, which will also be used in matching.
//
// Every Detector-based class must match the detect method signature.
//

class DSDetector
{

public:

	DSDetector() {};
	virtual ~DSDetector() {}

	// Main method to be implemented in your Detector.
	// Given input frame return vector of object-class bounding boxes.
	virtual void detect(const cv::Mat &frame, std::vector<CVUtils::BBoxf> &bboxes) = 0;

};
