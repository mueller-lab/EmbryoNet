#pragma once

#include "Encoder.hpp"
#include "DSEncoder.h"

class ABEncoder : public DSEncoder
{
	std::shared_ptr<Encoder> m_encoder;

public:

	ABEncoder(const std::shared_ptr<Encoder> &encoder);
	~ABEncoder();

	void encode(const cv::Mat &img, const std::vector<CVUtils::BBoxf> &bboxes, std::vector<std::vector<float> > &features);
};
