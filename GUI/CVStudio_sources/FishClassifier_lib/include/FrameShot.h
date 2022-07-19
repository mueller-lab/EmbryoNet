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

struct EmbryoData
{
    int trackID;
    int severity;
    std::vector<std::pair<int, float>> severityGroupProbs;
    CVUtils::BBoxf bbox;
    unsigned int classID;
    std::vector<float> classificationResult;
    std::vector<float> oldClassificationResult;
    std::string className;
    
};

struct FrameShot
{
    unsigned int frameId;
    std::vector<EmbryoData> embryoDatas;
    cv::Size frameSize;
};
