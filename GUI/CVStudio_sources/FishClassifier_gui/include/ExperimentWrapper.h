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

#include "EmbryoExperimentRunner.h"

class ExperimentWrapper : public QObject
{
	Q_OBJECT
	nlohmann::json m_classesJSON;

public:

	ExperimentWrapper(QObject* parent = nullptr);
	~ExperimentWrapper();

public slots:

	void runExperiment(const QString& path);
	void runTestStandalone();
	void recieveUserSettings(const QString& json);
	
	

signals:

	void imageProcessed(const std::string& imagePath, const std::string& jsonResult);
	void experimentStopped();
	void sendClasses(std::vector <std::pair<QString, QColor>>);
	void sendProgressPercent(const int& percent);
	void clearAnnovis();
	void userSettingsApplied();
private:

	std::vector <std::pair<QString, QColor>> loadClasses(const nlohmann::json& json);
	std::unique_ptr<EmbryoExperimentRunner> m_experimentRunner;

	nlohmann::json additionalSettings;
	void saveExperimentConfig(const nlohmann::json& globalConfig, const nlohmann::json& additionalConfig, const QString& pathToSave);

private:

	nlohmann::json updateDefaultConfig(const nlohmann::json& defaultSettings, const nlohmann::json& additionalSettings);

signals:

	 void onError(const QString& error);
};
