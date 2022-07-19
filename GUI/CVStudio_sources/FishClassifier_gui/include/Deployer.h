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

#include "MainWindow.h"
#include "Loader.h"
#include "ExperimentWrapper.h"

#include <QThread>

class Deployer: public QObject
{
	Q_OBJECT

private:

	QThread* m_experimentWrapperThread = nullptr;
	QThread* m_loaderThread = nullptr;

	Loader* m_loader = nullptr;
	MainWindow* m_mainWindow = nullptr;
	ExperimentWrapper* m_experimentWrapper = nullptr;
public:

	Deployer(MainWindow* mainWindow, QObject* parent = nullptr);
	~Deployer();
	
	void create();
	void connect();
};