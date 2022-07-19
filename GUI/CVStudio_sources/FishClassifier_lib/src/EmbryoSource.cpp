#include "EmbryoSource.h"
#include "Stream/PathStreamer.hpp"

EmbryoSource::EmbryoSource(const nlohmann::json& config)
{
    const std::string type = config["type"];
    if (type == "Path")
    {
        const std::string folder = config["folder"];
        m_streamer.reset(new CVUtils::PathStreamer(folder));
    }
}

EmbryoSource::~EmbryoSource()
{
}

std::pair<cv::Mat, std::string> EmbryoSource::operator() ()
{
    cv::Mat result;

    if (!m_streamer)
    {
        return { result, std::string() };
    }
    cv::String currentPath; 
    CVUtils::PathStreamer* pathStreamerPtr = dynamic_cast<CVUtils::PathStreamer*>(m_streamer.get());
    while (m_streamer->isOpened())
    {
        CVUtils::Image tmp;
        currentPath = pathStreamerPtr->currentPath();
        (*m_streamer) >> tmp;
        if (!tmp.empty())
        {
            result = tmp.matRef();
            break;
        }
    }

    if (!pathStreamerPtr)
    {
        throw std::runtime_error("pathStreamerPtr is null");
    }

    return { result, currentPath };
}

std::vector<std::string> EmbryoSource::getPathes() const
{
    const CVUtils::PathStreamer* pathStreamerPtr = dynamic_cast<CVUtils::PathStreamer*>(m_streamer.get());
    return pathStreamerPtr->getPaths();
}
