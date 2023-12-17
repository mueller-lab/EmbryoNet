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
#include "SettingsWidget.h"
#include "AnnoVis.h"
#include "AnnoWidget.h"
#include "EmbryoCounterWidget.h"
#include "ConfigWidget.h"

#include <QListWidget>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QLineEdit>


class ConfigWidget;

class MainWindow: public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:

	SettingsWidget* m_settingsWidget = nullptr;
	AnnoWidget* m_annoWidget = nullptr;
	AnnoVis* m_annoVis = nullptr;
	QLabel* m_imageNamesLabel = nullptr;
	QLabel* m_experimentNamesLabel = nullptr;
	QListWidget* m_imageList = nullptr;
	QListWidget* m_experimentList = nullptr;
	EmbryoCounter* m_embryoCounter = nullptr;
	
	QHBoxLayout* m_loadStartLayout = nullptr;
	QPushButton* m_loadButton = nullptr;
	QPushButton* m_startButton = nullptr;
	QPushButton* m_settingsButton = nullptr;
	QPushButton* m_licenseButton = nullptr;

	QVBoxLayout* m_progressBarLayout = nullptr;
	QLabel* m_progressBarLabel = nullptr;
	QProgressBar* m_loadProgressBar = nullptr;
	QLabel* m_algoritmProgressBarLabel = nullptr;
	QProgressBar* m_algoritmProgressBar = nullptr;

	
	QVBoxLayout*  m_configVLayout = nullptr;
	ConfigWidget* m_configWidget = nullptr;
	QPushButton* m_infoHelpButton = nullptr;
	QVBoxLayout* m_rightButtonsLayout = nullptr;
	QVBoxLayout* m_leftButtonLayout = nullptr;

	//QLabel* m_folderPathLabel = nullptr;
	QGridLayout* m_mainLayout = nullptr;
	QVBoxLayout* m_experimentInfoLayout = nullptr;
	QGridLayout* m_experimentImageLayout = nullptr;

private:

	int m_currentExperimtentIndex = 0;
	bool m_wereListsBlocked = false;
	std::map<QString, QStringList> m_loadedExperiments;

private:
	std::atomic <bool> m_runningExperiment  { false };
	void allocate();
	void draw();

	void connectInternal();
	
	void prepareLoad();

	void itemChanged(const int& currentRow);
	void experimentChanged(const int& currentRow);

	void blockLists();
	void restoreLists();

	void blockButtons();
	void restoreButtons();
	
	void blockConnectonsOnLoad();

	void blockListConnections();
	void restoreListConnections();



public slots:

	void closeEvent(QCloseEvent* event) override;

	void setStringList(const QStringList& stringList);
	void setExperimentStringList(const QStringList& stringList);
	void loadPicture(const QString& path);
	void showProcessedImage(const std::string & pathToImg , const std::string& json);
	void startProcessing();
	void stopProcessing();
	
	void updateAlgoritmProgressBar(const int& progress);
	void updateLoadProgressBar    (const int& progress);

	void restoreConnectonsOnLoad();
	void recieveClasses(const std::vector <std::pair<QString, QColor>>&);
	void clearAnnovis();

	void showQMessageBox(const QString& s);
	void onError(const QString& s);

	

signals:
	void userSettingsApplied();
	void sendDirectory(const QString& inputPath);
	void requestImageList(const QString& path);
	void startProcess(const QString& inputPath);
	void sendSettings(const QString& settings);
	
};
