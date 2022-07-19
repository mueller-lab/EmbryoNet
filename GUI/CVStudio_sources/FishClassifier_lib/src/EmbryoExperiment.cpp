#include "EmbryoExperiment.h"
#include "EmbryoLogic.h"
#include "EmbryoTracker.h"
#include "FrameShot.h"

#include "OpenCVNetManager.hpp"

EmbryoExperiment::EmbryoExperiment(const nlohmann::json& config)
{
    const auto& classiferConfig = config["classifier"];
 
    const std::string embryoClassifierType = config["classifier"]["type"];

    if (embryoClassifierType == "OpenCV")
    {
        m_classifier.reset(new EmbryoClassifier(classiferConfig));
    }
    else
    {
        throw std::runtime_error(std::string("Unsupported classifier type: ") + embryoClassifierType);
    }

    auto logicConfig = config["logic"];
    logicConfig["classes"] = config["classes"];
    m_logic.reset(new EmbryoLogic(logicConfig));

    m_classifier->setDefaultClass(m_logic->getClassIdFromName(m_classifier->getDefaultClassName()));

    const auto& trackerConfig = config["tracker"];
    m_tracker.reset(new EmbryoTracker(trackerConfig));
    m_frameId = 0;
}
        
EmbryoExperiment::~EmbryoExperiment()
{
}

void EmbryoExperiment::resetClasses(const nlohmann::json& json)
{
    this->parseClassesJson(json);
    m_classesJSON = json;
}

void EmbryoExperiment::parseClassesJson(const nlohmann::json& json)
{
    const int DEFAULT_SEVERITY = 100;
    const auto& pushToClass2SevereGroup = [this](int id, int curr_id)
    {
        if (!m_class2SevereGroup.count(id))
        {
            m_class2SevereGroup.emplace(id, std::vector<int>());
        }
        m_class2SevereGroup[id].push_back(curr_id);
    };

    int cur_id = 0;
    for (const auto& classJson : json)
    {
        int id = classJson["id"].get<int>();

        if (classJson.count("severities"))
        {
            for (const auto& s: classJson["severities"])
            {
                m_id2IDSeverityPair.emplace(cur_id, std::make_pair(id, s.get<int>()));
                pushToClass2SevereGroup(id, cur_id++);
            }
        }
        else
        {
            m_id2IDSeverityPair.emplace(cur_id, std::make_pair(id, DEFAULT_SEVERITY));
            pushToClass2SevereGroup(id, cur_id++);
        }
    }
}

std::string EmbryoExperiment::operator() (const cv::Mat& input, const float & age)
{
    const auto& trackerResult = m_tracker->processFrame(input);

    std::vector<CVUtils::Bboxf> trackBoxes;
    for (const auto& track : trackerResult.tracks)
    {
        trackBoxes.push_back(track.pos());
    }

    const auto& classifierResult = m_classifier->classify(input, trackBoxes, age);
    if (trackBoxes.size() != classifierResult.size())
    {
        throw std::runtime_error("trackBoxes.size() != classifierResult.size()");
    }

    FrameShot fs;
    fs.frameId = m_frameId;
    fs.frameSize = input.size();

    nlohmann::json detectionListJSON;
    for (size_t trackIdx = 0; trackIdx < trackBoxes.size(); ++trackIdx)
    {
     
        const auto& track = trackerResult.tracks[trackIdx];

        const auto id = track.id();

        const auto& xywh = track.pos().xywh();

        const int x = static_cast<int>(xywh[0]);
        const int y = static_cast<int>(xywh[1]);
        const int w = static_cast<int>(xywh[2]);
        const int h = static_cast<int>(xywh[3]);

        const int tlx = std::max(0, x);
        const int tly = std::max(0, y);;
        const int brx = std::min(tlx + w, input.cols);
        const int bry = std::min(tly + h, input.rows);
        
        const auto& probs = classifierResult[trackIdx];
        
        const size_t classID = std::distance(probs.begin(), std::max_element(probs.begin(), probs.end()));

        const size_t newId = m_id2IDSeverityPair[classID].first;
        const int severity = m_id2IDSeverityPair[classID].second;

        
        std::vector<float> newProbs;
        std::vector<std::pair<int, float>> severityProbs;
        for (auto it = m_class2SevereGroup.begin(); it != m_class2SevereGroup.end(); ++it)
        {
            std::vector<int> severeGroupIds = it->second;
            std::vector<float> severeGroupProbs;
            for (const int groupId : severeGroupIds)
            {
                severeGroupProbs.push_back(probs[groupId]);
                severityProbs.push_back(std::make_pair(m_id2IDSeverityPair[groupId].second, std::max<float>(0.000000001, probs[groupId])));
            }
            const float maxGorupProb =std::max<float>(0.000000001, *std::max_element(severeGroupProbs.begin(), severeGroupProbs.end()));


            newProbs.push_back(maxGorupProb);
        
        }
        
        std::string className = m_classesJSON[newId]["name"].get<std::string>();
        EmbryoData ed;
        ed.classID = newId;
        ed.classificationResult = newProbs;
        ed.bbox = track.pos();
        ed.trackID = track.id();
        ed.className = className;
        ed.severity = severity;
        ed.oldClassificationResult = classifierResult[trackIdx];
        ed.severityGroupProbs = severityProbs;
        fs.embryoDatas.push_back(ed);
       
        const auto& trackJSON = trackJsonData2Json(
            {
                id,
                newId,
                className,
                newProbs, 
                true,
                tlx,
                tly,
                brx,
                bry,
                severity,
                classifierResult[trackIdx],
                severityProbs
            }
        );


        detectionListJSON.push_back(trackJSON);
    }
    
    m_logic->feed(fs);

    m_frameId++;

    return detectionListJSON.empty() ? std::string("[]") : detectionListJSON.dump(4);
}

std::vector<std::string> EmbryoExperiment::operator()()
{
    std::vector<std::string> outputs;
    const auto& filteredDatas = m_logic->getFilteredDataByModels();

    for (const auto& filteredData : filteredDatas)
    {
        nlohmann::json detectionListJSON;
        const auto& tracks = filteredData.embryoDatas;

        for (size_t trackIdx = 0; trackIdx < tracks.size(); ++trackIdx)
        {
            const auto& track = tracks[trackIdx];
            const auto id = (size_t) track.trackID;

            const auto& xywh = track.bbox.xywh();

            const int x = static_cast<int>(xywh[0]);
            const int y = static_cast<int>(xywh[1]);
            const int w = static_cast<int>(xywh[2]);
            const int h = static_cast<int>(xywh[3]);

            const int tlx = std::max(0, x);
            const int tly = std::max(0, y);;
            const int brx = std::min(tlx + w, filteredData.frameSize.width);
            const int bry = std::min(tly + h, filteredData.frameSize.height);

            const auto& probs = track.classificationResult;

            const size_t classID = track.classID;
            const std::string className = m_classesJSON[track.classID]["name"].get<std::string>();

            const int severity = track.severity;

            const auto& trackJSON = trackJsonData2Json(
                {
                    id,
                    classID,
                    className,
                    probs,
                    true,
                    tlx,
                    tly,
                    brx,
                    bry,
                    severity,
                    track.oldClassificationResult,
                    track.severityGroupProbs
                }
            );
            detectionListJSON.push_back(trackJSON);
        }
        outputs.push_back(detectionListJSON.empty() ? std::string("[]") : detectionListJSON.dump(4));

    }
    return outputs;
}



nlohmann::json EmbryoExperiment::trackJsonData2Json(const TrackJsonData& trackJsonData)
{
    nlohmann::json trackJSON;

    trackJSON["id"] = trackJsonData.id;
    trackJSON["tlx"] = trackJsonData.tlx;
    trackJSON["tly"] = trackJsonData.tly;
    trackJSON["brx"] = trackJsonData.brx;
    trackJSON["bry"] = trackJsonData.bry;

    trackJSON["severe"] = trackJsonData.severity;
    trackJSON["oldProbs"] = trackJsonData.oldProbs;
    trackJSON["severityProbs"] = trackJsonData.severityProbs;


    nlohmann::json probJSON;
    for (const auto& singleProb : trackJsonData.probs)
    {
        probJSON.push_back(singleProb);
    }

    trackJSON["prob"] = probJSON;
    trackJSON["confidence"] = true;

    trackJSON["classId"] = trackJsonData.classID;
    trackJSON["className"] = trackJsonData.className;

    return trackJSON;
}
