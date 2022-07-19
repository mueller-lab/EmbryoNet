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

#include <opencv2/core.hpp>
#include <Bbox.hpp>

#define WITH_ONNXRUNTIME 0

#if WITH_ONNXRUNTIME
#include "onnxruntime_c_api.h"
#include "onnxruntime_cxx_api.h"
#include "tensorrt_provider_factory.h"
#endif

#include "EmbryoClass.h"

class NetManager;

class EmbryoClassifier
{
public:
    
    EmbryoClassifier(const nlohmann::json& config);
    ~EmbryoClassifier();

    std::vector<std::vector<float> > classify(const cv::Mat& frame,
        const std::vector<CVUtils::BBoxf>& boxes,
        const float& age);

    void resetClasses(const nlohmann::json& config);

    void setDefaultClass(const int defalutClass);
    std::string getDefaultClassName() const;
private:
    bool m_isrun = false;
    int m_outputClasses = 0;

    int m_defaultClass = 0;
    std::string m_default_class_name = "UNKNOWN";
    std::unique_ptr<cv::Size> m_size;

    std::unique_ptr<cv::Mat> m_minus;
    std::unique_ptr<cv::Mat> m_divide;

    std::vector<cv::Mat> generateFlippedImages(const cv::Mat& image);
#if WITH_ONNXRUNTIME

    Ort::SessionOptions* m_sessionOptions = nullptr;
    Ort::Session* m_session = nullptr;
    OrtMemoryInfo *m_memoryInfo = nullptr;
    OrtMemoryInfo* m_memoryInfo_t_for_conv = nullptr;
    OrtMemoryInfo* m_memoryInfo_t_for_fc = nullptr; 
    std::vector<std::vector<int64_t> > m_inpuNodeDims;
    Ort::Unowned<Ort::TensorTypeAndShapeInfo>* m_tensorInfo = nullptr;
    size_t m_shapeLen;
    Ort::Env m_env;

#else
    std::unique_ptr<NetManager> m_netManager;
#endif

    std::vector <EmbryoClass> m_embryoClasses;

};

