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
#include "EmbryoClass.h"

namespace cv
{
    class Mat;
}

class ResultRepresentor 
{

public:

    class RepresentImpl;

    ResultRepresentor(const nlohmann::json& config, const std::string& modelName = std::string());
    ~ResultRepresentor();

    void operator() (const cv::Mat& image, const std::string& resultJSON);

    void resetClasses(const nlohmann::json& colorJSON);

    
    void setRun(bool run);

    std::string getModelName() const;

   

private:
    std::string m_modelName;
    std::string m_resultJSON;

    std::vector<std::shared_ptr<RepresentImpl> > m_impls;

};
