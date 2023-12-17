#include "ResultRepresentor.h"

#include <opencv2/opencv.hpp>

#include <fstream>
#include "boost/filesystem.hpp"


namespace
{

std::string basename(const std::string& path)
{
    std::string result = path;
    std::replace(result.begin(), result.end(), '\\', '/');
    auto slash = result.find_last_of("/");
    if (slash == std::string::npos)
    {
        slash = 0;
    } else {
        slash += 1;
    }

    size_t punkt = result.find_last_of(".");
    if (slash == std::string::npos)
    {
        punkt = 0;
    }

    result = result.substr(slash, result.size() - slash - (result.size() - punkt));
    return result;
}

}

class ResultRepresentor::RepresentImpl
{

public:

    struct Data
    {
        cv::Mat image;
        nlohmann::json resultJSON;
    };

    virtual void construct(const nlohmann::json& config, const std::string& modelName) = 0;
    virtual void apply(const Data& data) = 0;
    
    void resetClasses(const nlohmann::json& config)
    {
        const auto& toEmbryoClass = [](const nlohmann::json& colorJSON) -> std::pair<EmbryoClass, cv::Scalar>
        {
            std::vector<int> color;

            for (const auto& floatElem : colorJSON["color"])
            {
                color.push_back(static_cast<int>(floatElem));
            }

            const std::string name = colorJSON["name"];
            EmbryoClass embryo = { colorJSON["id"], name, color };

            if (color.size() != 3)
            {
                throw std::runtime_error("Expected 3 channels in color");
            }

            const cv::Scalar embryoColor(color[0], color[1], color[2]);
            return { embryo, embryoColor };
        };


        for (const auto& embryoClass : config)
        {
            const auto& embryoClassColorPair = toEmbryoClass(embryoClass);
            m_embryoClasses.push_back(embryoClassColorPair.first);
            m_embryoColors.push_back(embryoClassColorPair.second);
        }

    }

    void setRun(bool run)
    {
        m_run = run;
    }

protected:
     
    std::vector<EmbryoClass> m_embryoClasses;
    std::vector<cv::Scalar> m_embryoColors;
    std::string m_modelName;

    bool m_run{ false };

    cv::Mat drawResult(const ResultRepresentor::RepresentImpl::Data&  data)
    {
        const auto& dataImage = data.image;
        const auto& detectionListJSON = data.resultJSON["detection_list"];

        cv::Mat drawImage = dataImage.clone();

        for (const auto& detectionJSON : detectionListJSON)
        {
            const int tlx = static_cast<int>(detectionJSON["tlx"]);
            const int tly = static_cast<int>(detectionJSON["tly"]);
            const int brx = static_cast<int>(detectionJSON["brx"]);
            const int bry = static_cast<int>(detectionJSON["bry"]);

            const cv::Point tl(tlx, tly);
            const cv::Point br(brx, bry);
            const cv::Rect bboxRect(tl, br);

            const float starOffset = 0.8;
            const cv::Point starPosition = starOffset * tl + (1.0f - starOffset) * br;

            const auto& probsJSON = detectionJSON["prob"];
            
            cv::Scalar embryoColor;
            for (size_t i = 0; i < probsJSON.size(); ++i)
            {
                embryoColor += static_cast<float>(probsJSON[i]) * m_embryoColors[i];
            }
            
            cv::rectangle(drawImage, bboxRect, embryoColor, 3);
            const int id = static_cast<int>(detectionJSON["id"]);

            const cv::Point bboxCenter = 0.5 * (tl + br);
            cv::putText(drawImage, std::to_string(id), bboxCenter, 
                       cv::FONT_HERSHEY_SIMPLEX, 1.0, embryoColor, 2);

            if (detectionJSON.count("isKeyFrame") && detectionJSON.get<bool>("isKeyFrame"))
            {
                cv::putText(drawImage, "*", starPosition,
                    cv::FONT_HERSHEY_SIMPLEX, 1.0, embryoColor, 2);
            }
        }

        return drawImage;
    }

};

class ImshowRepresenter : public ResultRepresentor::RepresentImpl
{

public:

    inline virtual void construct(const nlohmann::json& config, const std::string& modelName) override
    {
        m_modelName = modelName;
        const std::string name = config["name"];
        m_name = name;
        m_size = cv::Size(static_cast<int>(config["width"]), static_cast<int>(config["height"]));
        m_waitKey = static_cast<int> (config["waitKey"]);
    }

    virtual void apply(const Data& data) override
    {
        m_disp = drawResult(data);
        cv::resize(m_disp, m_disp, m_size);
        cv::imshow(m_name, m_disp);
        cv::waitKey(m_waitKey);
    }

private:

    std::string m_name;
    cv::Size m_size;
    int m_waitKey;

    cv::Mat m_disp;
};

class ImwriteRepresenter : public ResultRepresentor::RepresentImpl
{

public:

    inline virtual void construct(const nlohmann::json& config, const std::string &modelName) override
    {
        m_modelName = modelName;
        const std::string prefix = config["prefix"];
        m_prefix = prefix;
        const std::string folder = config["folder"];
        m_folder = folder;
    }
    

    virtual void apply(const Data& data) override
    {
        if (m_folder.empty())
        {
            return;
        }

        const std::string source_name = data.resultJSON["source_name"];
        std::string fullPathName = m_folder + std::string("/") + m_prefix + basename(source_name) + std::string(".jpg");
        cv::imwrite(fullPathName, drawResult(data));
    }

private:

    std::string m_prefix;
    std::string m_folder;
};

class JsonRepresenter : public ResultRepresentor::RepresentImpl
{

public:

    inline virtual void construct(const nlohmann::json& config, const std::string& modelName) override
    {
        m_modelName = modelName;
        const std::string folder = config["folder"];
        m_folder = folder;
    }

    virtual void apply(const Data& data) override
    {
        if (m_folder.empty()) {
            return;
        }

        const std::string source_name = data.resultJSON["source_name"];
        std::ofstream fileJson(m_folder + std::string("/") + basename(source_name) + std::string(".json"));
        fileJson << data.resultJSON.dump(4);
    }

private:

    std::string m_folder;

};

class LabelerRepresenter: public ResultRepresentor::RepresentImpl
{
public:

    inline virtual void construct(const nlohmann::json& config, const std::string& modelName) override
    {
        m_modelName = modelName;
        const std::string experiment_folder = config["experiment_folder"];
        m_experiment_folder = experiment_folder;
        
        m_classesJSON = config["classes"];
    }

    virtual void apply(const Data& data) override
    {

        if (m_experiment_folder.empty()) {
            return;
        }

        const boost::filesystem::path experimentFolderPath(m_experiment_folder);
        boost::filesystem::path json_folder;
        boost::filesystem::path class_agnostic_json_folder;

        if (!m_run) 
        {
            json_folder = experimentFolderPath / boost::filesystem::path(m_modelName +  "_Classified_result_1_json"); //todo: add a name
        }
        else 
        {
            json_folder = experimentFolderPath / boost::filesystem::path(m_modelName + "_Classified_result_1_json_corrected");
        }
        
        class_agnostic_json_folder = experimentFolderPath / boost::filesystem::path("Classified_result_1_json_class_agnostic");

        if (!boost::filesystem::is_directory(json_folder))
        {
            boost::filesystem::create_directory(json_folder);
        }

        if (!boost::filesystem::is_directory(class_agnostic_json_folder))
        {
            boost::filesystem::create_directory(class_agnostic_json_folder);
        }
      
        nlohmann::json resultJSON = data.resultJSON;
        nlohmann::json resultClassAgnosticJSON = data.resultJSON;
     
        for (auto& detectionJSON: resultJSON["detection_list"])
        {
           std::vector<float> probs = detectionJSON["prob"];
           
           const int classID = static_cast<int>(detectionJSON["classId"]);

           detectionJSON["className"] = m_classesJSON[classID]["name"];

        }

        for (auto& detectionJSON : resultClassAgnosticJSON["detection_list"])
        {
             detectionJSON["className"] = std::string("UNKNOWN"); //to do : redo 
             detectionJSON["classId"] = 3;
        }
       
        resultJSON["classes"] = m_classesJSON;
        resultJSON["labeler"] = "default";

        std::string sourceName = resultJSON["source_name"];
        resultJSON["source_name"] = boost::filesystem::basename(sourceName) + boost::filesystem::extension(sourceName);

        resultClassAgnosticJSON["classes"] = m_classesJSON;
        resultClassAgnosticJSON["labeler"] = "default";

        sourceName = resultClassAgnosticJSON["source_name"].get<std::string>();
        resultClassAgnosticJSON["source_name"] = boost::filesystem::basename(sourceName) + boost::filesystem::extension(sourceName);

        const std::string source_name = data.resultJSON["source_name"];
        std::ofstream fileJson(json_folder.string() + std::string("/") + boost::filesystem::basename(source_name) + std::string(".json"));
        std::ofstream fileClassAgnosticJson(class_agnostic_json_folder.string() + std::string("/") + boost::filesystem::basename(source_name) + std::string(".json"));
        fileJson << resultJSON.dump(4);
        fileClassAgnosticJson << resultClassAgnosticJSON.dump(4);
    }

private:

 
    std::string m_experiment_folder;
    nlohmann::json m_classesJSON;
};

ResultRepresentor::ResultRepresentor(const nlohmann::json& config, const std::string& modelName)
{

    m_modelName = modelName;

    for (const auto& representerConfig : config)
    {
        std::shared_ptr<ResultRepresentor::RepresentImpl> impl;

        const std::string type = representerConfig["type"];
        if (type == "imshow")
        {
            impl.reset(new ImshowRepresenter());
        }
        if (type == "imwrite")
        {
            impl.reset(new ImwriteRepresenter());
        }
        if (type == "json")
        {
            impl.reset(new JsonRepresenter());
        }
        if (type == "labeler")
        {
            impl.reset(new LabelerRepresenter());
        }

        impl->construct(representerConfig, m_modelName);
        m_impls.push_back(impl);
    }
}

ResultRepresentor::~ResultRepresentor()
{
}

void ResultRepresentor::operator() (const cv::Mat& image, const std::string& resultJSON)
{
    const ResultRepresentor::RepresentImpl::Data data{ image, nlohmann::json::parse(resultJSON) };
    for (const auto& impl : m_impls)
    {
        impl->apply(data);
    }
}

void ResultRepresentor::resetClasses(const nlohmann::json& colorJSON)
{
    for (auto& impl : m_impls)
    {
        impl->resetClasses(colorJSON);
    }
}

void ResultRepresentor::setRun(bool run)
{
    for (auto& impl : m_impls)
    {
        impl->setRun(run);
    }
}

std::string ResultRepresentor::getModelName() const
{
    return m_modelName;
}
