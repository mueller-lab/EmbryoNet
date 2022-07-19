#include "EmbryoTracker.h"
#include "EmbryoDetector.h"


EmbryoTracker::EmbryoTracker(const nlohmann::json& config)
{
    const auto& detecorConfig = config["detector"];
    m_detector.reset(new EmbryoDetector(detecorConfig));
    m_tracker.reset(new DeepSort(m_detector, nullptr));
}

EmbryoTracker::~EmbryoTracker()
{
}

TrackerData EmbryoTracker::processFrame(const cv::Mat& input)
{
    m_tracker->processFrame(input);
    return
    {
        m_tracker->tracks(),
        m_tracker->bboxes()
    };
}
