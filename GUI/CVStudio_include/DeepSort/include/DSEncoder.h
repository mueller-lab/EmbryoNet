#pragma once

#include "Bbox.hpp"

//
// DSEncoder abstract interface
//
// DSEncoder is an algorithm, that given an image and a bounding box of the detection in that image
// generates a vector of floating point features, which DeepSort uses during feature-based matching.
//
// Every DSEncoder-based class you pass to the DeepSort algorithm must match the DSEncoder::match signature.
//
class DSEncoder
{
public:

	// Main method to be implemented in your Encoder.
	// Given an input frame and a vector of object-class bounding boxes 
	// return vector of features for each bounding box.
	virtual void encode(const cv::Mat &img, const std::vector<CVUtils::BBoxf> &bboxes, std::vector<std::vector<float> > &features) = 0;
	virtual ~DSEncoder() {}

};
