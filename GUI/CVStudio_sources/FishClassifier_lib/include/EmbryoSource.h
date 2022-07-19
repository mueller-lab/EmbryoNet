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
#include <opencv2/opencv.hpp>

#include "nlohmann/json.hpp"

namespace CVUtils
{

class Streamer;

}

class EmbryoSource
{

public:

    EmbryoSource(const nlohmann::json& config);
    ~EmbryoSource();

    std::pair<cv::Mat, std::string> operator() ();
    std::vector<std::string> getPathes() const;

private:

    std::unique_ptr<CVUtils::Streamer> m_streamer;

};

