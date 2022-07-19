#pragma once

#include "AlgorithmBase.hpp"

class SingleImageAlgorithm: public AlgorithmBase
{
public:

	SingleImageAlgorithm(const std::string &name, const std::string &type, const std::string &engine);
	virtual ~SingleImageAlgorithm();

	virtual void run(const std::vector<CVUtils::Image> &input);
	virtual void run(const CVUtils::Image &input) = 0;
};