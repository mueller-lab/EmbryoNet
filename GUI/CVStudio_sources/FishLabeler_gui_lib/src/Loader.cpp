//=======================================================================================================================
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "Loader.h"

#include "ChooseFolderWidget.h"

#include "SaveWidget.h"

#include "Stream/PathStreamer.hpp"

#include <QMessageBox>

#include <boost/filesystem.hpp>

namespace
{
    std::unique_ptr<CVUtils::PathStreamer> streamer;
} // namespace

Loader::Loader(QObject* parent) : QObject(parent)
{
}

Loader::~Loader()
{
}

void Loader::load(const QString& path)
{
    m_currentDir = path;
    boost::filesystem::path pathToImageFolder(boost::filesystem::path(path.toStdString()) / "images");

    if (!boost::filesystem::is_directory(pathToImageFolder))
    {
        QMessageBox b;
        b.setText(QString("No image directory was found in dir : ") + path);
        b.exec();
        return;
    }

    const boost::filesystem::directory_iterator end_itr;

    std::vector<std::string> jsonFolders;
    for (boost::filesystem::directory_iterator itr(path.toStdString()); itr != end_itr; ++itr)
    {
        std::string lowerPath = itr->path().string();
        std::for_each(lowerPath.begin(), lowerPath.end(), [](char& c) { c = std::tolower(c); });

        if (boost::filesystem::is_directory(itr->path()) && 
            lowerPath.find("json") != std::string::npos)
        {
            jsonFolders.push_back(itr->path().string());
        }
    }

    if (jsonFolders.empty())
    {
        QMessageBox b;
        b.setText(QString("No json folders were found in dir : ") + path);
        b.exec();
        return;
    }

    
    QStringList jsontPathsStringList;

    std::for_each(jsonFolders.begin(), jsonFolders.end(),
                  [&jsontPathsStringList](const std::string& jsonPath) 
                  {
                      jsontPathsStringList.push_back(QString::fromStdString(jsonPath));
                  });

    ChooseFolderWidget chooseFolderWidget;
    chooseFolderWidget.setFolderList(jsontPathsStringList);
    std::string chosenJSONFolder;
    QObject::connect
    (
        &chooseFolderWidget,
        &ChooseFolderWidget::sendFolder,
        [&chosenJSONFolder] (const QString& jsonFolder)
        {
            chosenJSONFolder = jsonFolder.toStdString();
        }
    );

    chooseFolderWidget.exec();

    boost::filesystem::path pathToJsonFolder = chosenJSONFolder;
    if (!boost::filesystem::is_directory(chosenJSONFolder))
    {
        return;
    }

    boost::filesystem::directory_iterator jsonIterator (pathToJsonFolder);
    boost::filesystem::directory_iterator imageIterator(pathToImageFolder);
    int jsonscnt = std::count_if(jsonIterator, boost::filesystem::directory_iterator(), 
           static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file));

    int imagescnt = std::count_if(imageIterator, boost::filesystem::directory_iterator(),
        static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file));

    if (imagescnt != jsonscnt)
    {
        QMessageBox b;
        b.setText("number of files in image folder and in chosen json folder are not equal");
        b.exec();
        return;
    }

    std::vector<std::string> jsonFiles;
    for (boost::filesystem::directory_iterator itr(pathToJsonFolder); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path()))
        {
            const std::string& current_file = itr->path().string();
            if (current_file.find(".json") != std::string::npos)
            {
                jsonFiles.push_back(current_file);
            }
        }
    }

    std::vector<std::pair<std::string, nlohmann::json> > images2JSONs;
    for (const auto& jsonFile : jsonFiles)
    {
        nlohmann::json json;
        std::ifstream(jsonFile) >> json;
        const std::string& imagePathFromJSON = json["source_name"];

        const std::string imageNameWOExt = boost::filesystem::path(imagePathFromJSON).stem().string();
        
        const std::vector<std::string> possibleExtensions = {
            std::string(".jpg"),
            std::string(".tiff"),
            std::string(".tif"),
            std::string(".bmp"),
            std::string(".jpeg"),
            std::string(".png")
        };

        std::string imagePath;
        for (const std::string& possibleExtension : possibleExtensions)
        {
            auto currImagePath = pathToImageFolder / boost::filesystem::path(imageNameWOExt + possibleExtension);

            if (boost::filesystem::is_regular_file(currImagePath))
            {
                imagePath = currImagePath.string();
                break;
             }
        }

        if (imagePath.empty())
        {
            throw std::runtime_error("Image " + imagePath + " not found in image folder");
        }

       
        images2JSONs.push_back({ imagePath, json });
    }
    
    QStringList pathsToSend;

    for (const auto& image2JSON : images2JSONs)
    {
        pathsToSend.push_back(QString::fromStdString(image2JSON.first));
    }

    const auto& embryoClasses = loadClasses(images2JSONs);
    if (embryoClasses.empty())
    {
        return;
    }


    emit sendClasses(embryoClasses);
    emit sendImages2JSONs(images2JSONs);
    emit sendFrameList(pathsToSend);
    emit sendFrameCount(pathsToSend.size());
}

std::vector <std::pair<QString, QColor>> Loader::loadClasses(const std::vector<std::pair<std::string, nlohmann::json>>& jsons)
{
    std::vector<std::pair <QString, QColor>> existingClasses;
    std::string filename;

    for (const auto& currentJsonPair : jsons)
    {
        std::pair <QString, QColor> outputClass;
        nlohmann::json currentJson = currentJsonPair.second;
        std::vector<std::pair<QString, QColor> > outputClasses;
        const std::string fn = currentJson["source_name"];
        filename = fn;
        nlohmann::json classes = currentJson["classes"];
        for (const auto& embryoClass : classes)
        {
            const std::string clsName = embryoClass["name"];
            std::vector <int> color;
            for (const auto& c : embryoClass["color"])
            {
                color.push_back(static_cast<int> (c));
            }
            QColor clsColor = QColor(color[2], color[1], color[0]);
            outputClass = { QString::fromStdString(clsName), clsColor };
            outputClasses.push_back(outputClass);
        }

        if (existingClasses.empty())
        {
            existingClasses = outputClasses;
        }
        else if (existingClasses != outputClasses)
        {
            QMessageBox b;
            b.setText(QString::fromStdString("Classes in database are not equal. error appeared for source name " + filename));

            b.exec();
            
            m_loadedClasses = nlohmann::json();
            return decltype(existingClasses)();
        }
        m_loadedClasses = classes;
    }
    return existingClasses;
}

void Loader::save(const std::vector<std::pair<std::string, nlohmann::json> >& images2JSONs)
{

    if (images2JSONs.size()==0)
    {
        return;
    }
    SaveWidget choosaveWidget;
    std::string labeler;
    QObject::connect
    (
        &choosaveWidget,
        &SaveWidget::sendLabelerName,
        [&labeler](const QString& jsonFolder)
    {
        labeler = jsonFolder.toStdString();
    }
    );

    choosaveWidget.exec();
    if (labeler.size() == 0)
    {
        return;
    }

    boost::filesystem::path saveJsonDirectory = boost::filesystem::path(m_currentDir.toStdString()) / (labeler+"_json");

    if (boost::filesystem::is_directory(saveJsonDirectory))
    {
        QMessageBox b;
        b.setText("this name is already taken. Please choose another name");
        b.exec();
        return;

    }
    else{
        boost::filesystem::create_directory(saveJsonDirectory);
    }
    int imageId = 0;
    for (auto& imageJson : images2JSONs)
    {
        nlohmann::json renamedJson = imageJson.second;
        const std::string source_name = renamedJson["source_name"];
        renamedJson["labeler"] = labeler;
        renamedJson["classes"] = m_loadedClasses;
        renamedJson["source_type"] = "folder";
        renamedJson["imageId"] = imageId;
     
        imageId++;
        std::ofstream filestream(saveJsonDirectory.string() + std::string("/") + boost::filesystem::basename(source_name) + std::string(".json"));
        filestream << renamedJson.dump(4);
    }

}