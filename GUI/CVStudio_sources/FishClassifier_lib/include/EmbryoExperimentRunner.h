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

#include "nlohmann/json.hpp"

#include "EmbryoExperiment.h"

#include <memory>

class EmbryoSource;
class ResultRepresentor;

class EmbryoExperimentRunner
{
public:

    EmbryoExperimentRunner(const nlohmann::json& config);
    ~EmbryoExperimentRunner();

    void runStandAlone();

    std::pair<cv::Mat, std::string> runStep(std::pair<cv::Mat, std::string> imagePathPair);
    
    std::pair <cv::Mat, std::string> getNextImageJsonPair();

    inline bool isInitialOver() const { return m_isInitialOver; }
    inline std::vector<std::string> matPathes() const { return m_matPathes; };

    std::vector<std::string> getPaths() const;

private:
   
    std::unique_ptr<EmbryoExperiment> m_experiment;
    std::unique_ptr<EmbryoSource> m_source;
    std::unique_ptr<ResultRepresentor> m_resultRepresentor;

    std::vector<std::string> m_matPathes;
    std::vector<std::string> m_fixedJsonResults;
    int m_fixedJsonResultIndex = 0;
    bool m_isInitialOver = false;

    std::string m_microscopePattern;

    float m_startPoint = 0.0;
    float m_stopPoint = 1.0;
private:

    void construct(const nlohmann::json& config);


    float findAge(const std::string& imagePath);


    std::pair<cv::Mat, std::string> initialStep(std::pair<cv::Mat, std::string> imagePathPair);
    std::pair<cv::Mat, std::string> fixStep(std::pair<cv::Mat, std::string> imagePathPair);

};
