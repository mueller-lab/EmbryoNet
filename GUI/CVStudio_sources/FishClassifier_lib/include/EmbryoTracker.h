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

#include "DeepSort.h"
#include "TrackerData.h"

#include <memory>



class EmbryoDetector;

class EmbryoTracker 
{

    std::shared_ptr<EmbryoDetector> m_detector;
    std::shared_ptr<DeepSort> m_tracker;

public:

    EmbryoTracker(const nlohmann::json& config);
    ~EmbryoTracker();

    TrackerData processFrame(const cv::Mat& input);

};
