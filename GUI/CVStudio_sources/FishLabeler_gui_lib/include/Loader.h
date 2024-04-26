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
#include <QObject>
#include <QString>
#include "nlohmann/json.hpp"

class Loader: public QObject
{
    Q_OBJECT

       QString m_currentDir;
public:

    Loader(QObject* parent = nullptr);
    ~Loader();
    nlohmann::json m_loadedClasses;
  
public slots:

    std::vector <std::pair<QString, QColor>> loadClasses(const std::vector<std::pair<std::string, nlohmann::json>>&jsons);

    void load(const QString& path);
    void save(const std::vector<std::pair<std::string, nlohmann::json> >& images2JSONs);

signals:

    void sendImages2JSONs(const std::vector<std::pair<std::string, nlohmann::json> >&);
    void sendFrameList(const QStringList& stringList);
    void sendFrameCount(const unsigned int count);
    void sendClasses(const std::vector<std::pair<QString, QColor>>);

};
