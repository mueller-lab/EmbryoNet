//=======================================================================================================================
//Copyright
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================


#pragma once

#include <memory>

#include "nlohmann/json.hpp"

#include "EmbryoClassifier.h"

class EmbryoLogic;
class EmbryoTracker;

namespace cv
{
    class Mat;
}

struct TrackJsonData
{
    size_t id; 
    size_t classID;
    std::string className;
    std::vector<float> probs;
    bool confidence; 
    int tlx;
    int tly;
    int brx;
    int bry;
    int severity; 
    std::vector<float> oldProbs;
    std::vector<std::pair<int, float>> severityProbs;
};


class EmbryoExperiment
{

public:

    EmbryoExperiment(const nlohmann::json& config);
    ~EmbryoExperiment();

    std::string operator() (const cv::Mat& input, const float& age);

    std::vector<std::string> operator()();

    nlohmann::json trackJsonData2Json(const TrackJsonData& trackJsonData);

    void resetClasses(const nlohmann::json& json);

    void parseClassesJson(const nlohmann::json& json);

private:
    
    unsigned int m_frameId;
    std::unique_ptr<EmbryoTracker> m_tracker;
    std::unique_ptr<EmbryoClassifier> m_classifier;
    std::unique_ptr<EmbryoLogic> m_logic;
    nlohmann::json m_classesJSON;
    std::map <int, std::pair<int, int>> m_id2IDSeverityPair;
    std::map<int, std::vector<int> > m_class2SevereGroup;

};