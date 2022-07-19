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
#include "TrackerData.h"

#include "FrameShot.h"

struct EmbryoShot
{
    unsigned int frameId;
    unsigned int trackID; 
    std::vector<float> classificationResult;
    unsigned int classID;
    CVUtils::BBoxf bbox;
    bool certain;
    int severity;
};

using SeverityMap = std::map<unsigned int, std::map<size_t, size_t> >;

struct ModelWithBordersAndQuality
{
    std::vector<size_t> model;
    std::vector<unsigned int> borders;
    int quality;

    bool operator < (const ModelWithBordersAndQuality& other) const
    {
        return this->quality < other.quality;
    }
};

class EmbryoLogic
{
  
public: 

    EmbryoLogic(const nlohmann::json& config);
    ~EmbryoLogic();

    void feed(const FrameShot& fs);
   
    std::vector<FrameShot> getFilteredDataByModels();
    int getClassIdFromName(const  std::string& className) const ;

private:
    void runAnalysis();
    ModelWithBordersAndQuality findBestModel(const std::vector<EmbryoShot>& embryoShots);
    ModelWithBordersAndQuality calculateQuality(const std::vector <EmbryoShot>& embryoShots, const std::vector<size_t>& model);
    std::vector <EmbryoShot> overrideStates(const std::vector <EmbryoShot>& embryoShots, const ModelWithBordersAndQuality & model);
    SeverityMap getConcentrationsPerClass(const std::vector<EmbryoShot>& trackShots, const std::vector<int>& ignoreFrames);
    size_t specifySeverity(size_t classId, const SeverityMap& severityMap) const;

private:

    std::map<std::string, size_t> m_classesStrIdMap;

    std::map<std::string, size_t> m_classesStr_globalIdMap;
    
    std::vector<std::vector<size_t>> m_avaliableModels;
    
    std::map<int, std::vector<EmbryoShot>> m_inputEmbryoShots;
    std::map<int, std::vector<EmbryoShot>> m_outputEmbryoShots;

    std::vector<FrameShot> m_inputFrameShots;
    std::vector<FrameShot> m_outputFrameShots;
    

};
