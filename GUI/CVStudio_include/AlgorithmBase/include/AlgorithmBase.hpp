#pragma once

#include "Image.hpp"

#include <memory>

class AlgorithmBase
{
    const std::string m_name;
    const std::string m_type;
    const std::string m_engine;
    
public:

    AlgorithmBase(const std::string &name, const std::string &type, const std::string &engine);
    virtual ~AlgorithmBase();
    
    std::string name() const;
    std::string type() const;
    std::string engine() const;

    virtual void run(const std::vector<CVUtils::Image> &input) = 0;
};

static std::vector<std::string> AlgorithmTypes
{
	std::string("Detector"), std::string("Encoder"), std::string("Classifier")
};

void measureRuntime(std::shared_ptr<AlgorithmBase> algorithm, const std::vector<CVUtils::Image> &input, size_t iters = 100, size_t warmupIters = 0);