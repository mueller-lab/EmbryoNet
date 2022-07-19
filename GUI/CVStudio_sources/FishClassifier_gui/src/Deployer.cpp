//=======================================================================================================================
//Copyright
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "Deployer.h"

Deployer::Deployer(MainWindow* mainWindow, QObject* parent) : QObject(parent)
{
	m_mainWindow = mainWindow;

	create();
	connect();
}

Deployer::~Deployer()
{
}

void Deployer::create()
{
	m_loader = new Loader();
	m_loaderThread = new QThread;
	m_loader->moveToThread(m_loaderThread);
	m_loaderThread->start();

	m_experimentWrapper = new ExperimentWrapper();
	m_experimentWrapperThread = new QThread;
	m_experimentWrapper->moveToThread(m_experimentWrapperThread);
	m_experimentWrapperThread->start();
}

void Deployer::connect()
{
	QObject::connect(
		m_mainWindow,
		&MainWindow::sendDirectory,
		m_loader,
		&Loader::load
	);
	QObject::connect(
		m_loader,
		&Loader::sendError,
		m_mainWindow, 
		&MainWindow::showQMessageBox
	);

	
	QObject::connect(
		m_loader,
		&Loader::sendExperiments,
		m_mainWindow,
		&MainWindow::setExperimentStringList
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::startProcess,
		m_experimentWrapper,
		&ExperimentWrapper::runExperiment,
		Qt::QueuedConnection
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::requestImageList,
		m_loader,
		&Loader::loadFolder
	);

	QObject::connect(
		m_loader,
		&Loader::sendPathes,
		m_mainWindow,
		&MainWindow::setStringList
	);

	QObject::connect(
		m_loader,
		&Loader::sendProgressPercent,
		m_mainWindow,
		&MainWindow::updateLoadProgressBar
	);

	QObject::connect(
		m_experimentWrapper,
		&ExperimentWrapper::imageProcessed,
		m_mainWindow,
		&MainWindow::showProcessedImage,
		Qt::BlockingQueuedConnection
	);

	QObject::connect(
		m_experimentWrapper,
		&ExperimentWrapper::experimentStopped,
		m_mainWindow,
		&MainWindow::stopProcessing,
		Qt::QueuedConnection
	);


	QObject::connect(
		m_loader,
		&Loader::sendLoadFinished,
		m_mainWindow,
		&MainWindow::restoreConnectonsOnLoad,
		Qt::QueuedConnection
	);

	QObject::connect(
		m_experimentWrapper,
		&ExperimentWrapper::sendClasses,
		m_mainWindow,
		&MainWindow::recieveClasses,
		Qt::QueuedConnection

	);
	QObject::connect(
		m_experimentWrapper,
		&ExperimentWrapper::sendProgressPercent,
		m_mainWindow,
		&MainWindow::updateAlgoritmProgressBar
	);

	QObject::connect(
		m_experimentWrapper,
		&ExperimentWrapper::clearAnnovis,
		m_mainWindow,
		&MainWindow::clearAnnovis
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::sendSettings,
		m_experimentWrapper,
		&ExperimentWrapper::recieveUserSettings
	);

	QObject::connect(
		m_experimentWrapper,
		&ExperimentWrapper::userSettingsApplied,
		m_mainWindow,
		&MainWindow::userSettingsApplied
	);

	QObject::connect(
		m_experimentWrapper,
		&ExperimentWrapper::onError,
		m_mainWindow,
		&MainWindow::onError,
		Qt::BlockingQueuedConnection
	);




}
