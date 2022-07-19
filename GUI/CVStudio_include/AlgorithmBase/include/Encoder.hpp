#pragma once

#include "SingleImageAlgorithm.hpp"
#include "Bbox.hpp"

class Encoder : public SingleImageAlgorithm
{
protected:

	std::vector<std::vector<float> > m_features;
	std::vector<CVUtils::BBox> m_boxes;

public:

	Encoder(const std::string &name, const std::string &engine);
	virtual ~Encoder();

	void run(const CVUtils::Image &input);

	virtual void encode(const CVUtils::Image &input) = 0;

	void setBoxes(const std::vector<CVUtils::BBox> &boxes);
	std::vector<std::vector<float> > getFeatures() const;
};
