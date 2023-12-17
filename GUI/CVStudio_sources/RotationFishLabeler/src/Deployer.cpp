//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "Deployer.h"
#include <qthread.h>
Deployer::Deployer(MainWindow* mainWindow, QObject* parent) : QObject(parent), m_mainWindow(mainWindow)
{
	construct();
	connect();
}

Deployer::~Deployer()
{
}

void Deployer::construct()
{
	m_annovisThread = new QThread;
	
	m_loader = new Loader(this);
	m_annovis = new AnnoVis(this);
	m_annovis->moveToThread(m_annovisThread);
	m_annovisThread->start();
}

void Deployer::connect()
{

	QObject::connect(
		this->m_mainWindow,
		&MainWindow::selectAll,
		m_annovis,
		&AnnoVis::selectAll
	);

	QObject::connect(
		this->m_mainWindow,
		&MainWindow::unselectAll,
		m_annovis,
		&AnnoVis::unselectAll
	);

	QObject::connect(
		this->m_mainWindow,
		&MainWindow::highlightUnknowns,
		m_annovis,
		&AnnoVis::highlightUnknowns
	);

	QObject::connect(
		m_loader,
		&Loader::sendImages2JSONs,
		m_annovis,
		&AnnoVis::setImages2JSONs
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::itemChanged,
		m_annovis,
		&AnnoVis::redraw
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::sendDirectory,
		m_loader,
		&Loader::load
	);

	QObject::connect(
		m_loader,
		&Loader::sendFrameList,
		m_mainWindow,
		&MainWindow::setStringList
	);

	
	QObject::connect(
		m_annovis,
		&AnnoVis::sendImage,
		m_mainWindow,
		&MainWindow::showPicture
	);


	QObject::connect(
		m_mainWindow,
		&MainWindow::sendLeftClick,
		m_annovis,
		&AnnoVis::setHighlighted
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::wheelPress,
		m_annovis,
		&AnnoVis::wheelPress
	);


	QObject::connect(
		m_mainWindow,
		&MainWindow::sendWheelRotation,
		m_annovis,
		&AnnoVis::getWheelRotation
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::sendConcentration,
		m_annovis,
		&AnnoVis::setConcentration
	);

	QObject::connect(
		m_loader,
		&Loader::sendClasses,
		m_mainWindow,
		&MainWindow::recieveClasses
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::sendClass,
		m_annovis,
		&AnnoVis::setNewClass
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::sendEscape,
		m_annovis,
		&AnnoVis::clearTracks
	);

	QObject::connect(
		m_annovis,
		&AnnoVis::sendCounters,
		m_mainWindow,
		&MainWindow::recieveCounters
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::saveRequest,
		m_annovis,
		&AnnoVis::processSaveRequest
	);

	QObject::connect(
		m_annovis,
		&AnnoVis::save,
		m_loader,
		&Loader::save
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::sendRotation,
		m_annovis,
		&AnnoVis::setRotation
	);

	QObject::connect(
		m_annovis,
		&AnnoVis::sendRotation, 
		m_mainWindow,
		&MainWindow::getRotation
	);

}
