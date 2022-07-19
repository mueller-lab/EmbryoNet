//=======================================================================================================================
//Copyright
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================



#include "Loader.h"
#include <QDir>

#include <boost/filesystem.hpp>

#include <opencv2/opencv.hpp>

Loader::Loader(QObject * parent):QObject(parent)
{
}

Loader::~Loader()
{
}

void Loader::load(const QString& path)
{
    const boost::filesystem::path boostPath(path.toStdString());
    if (!boost::filesystem::is_directory(boostPath))
    {
        emit sendError(QString("No directory was found in dir : ") + path);
 
        return;
    }
    QStringList experimentFolders;
    const boost::filesystem::recursive_directory_iterator rend_itr;
    const boost::filesystem::directory_iterator end_itr;
    int percent = 0;
    for (boost::filesystem::recursive_directory_iterator itr(boostPath); itr != rend_itr; ++itr)
    {
        if (boost::filesystem::is_directory(itr->path()))
        {
            for (boost::filesystem::directory_iterator innerItr(itr->path()); innerItr != end_itr; ++innerItr)
            {
                if (boost::filesystem::is_directory(innerItr->path()) &&
                    boost::filesystem::basename(innerItr->path().string()) == std::string("images"))
                {
                    std::string experimentFolderPath = itr->path().string();
                    std::replace(experimentFolderPath.begin(), experimentFolderPath.end(), '\\', '/');
                    experimentFolders.push_back(QString::fromStdString(experimentFolderPath));
                    emit this->sendProgressPercent(percent);
                    percent += 10;
                    if (percent > 100)
                    { 
                        percent = 0;
                    }
                    break;
                }
            }
        }
    }

    if (experimentFolders.empty())
    {
        emit sendError(QString("No experiment folder was found in dir : ") + path);

        return;
    }
    emit this->sendExperiments(experimentFolders);
}

void Loader::loadFolder(const QString& path)
{
    const boost::filesystem::path pathToImageFolder(boost::filesystem::path(path.toStdString()) /
                                                    std::string("images"));

    if (!boost::filesystem::is_directory(pathToImageFolder))
    {
        emit sendError(QString("No image directory was found in dir : ") + path);
        emit sendLoadFinished();
        return;
    }

    const float numFilesInFolder = static_cast<float>(std::count_if(
        boost::filesystem::directory_iterator(pathToImageFolder),
        boost::filesystem::directory_iterator(),
        static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file)));

    float currentFileIdx = 0;

    QStringList imageFiles;
    const boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(pathToImageFolder); itr != end_itr; ++itr)
    {   
        if (is_regular_file(itr->path()) && cv::imread(itr->path().string()).data)
        {
            std::string imageFilePath = itr->path().string();
            std::replace(imageFilePath.begin(), imageFilePath.end(), '\\', '/');
            imageFiles.push_back(QString::fromStdString(imageFilePath));
        }
        ++currentFileIdx;
        emit this->sendProgressPercent(static_cast<int>(currentFileIdx / numFilesInFolder * 100.0f + 0.5f));
    }
    if (imageFiles.empty()){

        emit sendError(QString("No images were found in : ") + path);
        emit sendLoadFinished();
        return;
    }

    emit this->sendLoadFinished();
    emit this->sendPathes(imageFiles);
}