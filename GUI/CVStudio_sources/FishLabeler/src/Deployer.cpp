//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "Deployer.h"

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
	m_loader = new Loader(this);
	m_player = new PlayerOld(this);
	m_annovis = new AnnoVis(this);
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
		m_loader,
		&Loader::sendFrameCount,
		m_mainWindow,
		&MainWindow::setSlider
	);

	QObject::connect(
		m_loader,
		&Loader::sendFrameCount,
		m_player,
		&PlayerOld::setFramesLength
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::startButtonClicked,
		m_player,
		&PlayerOld::play
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::stopButtonClicked,
		m_player,
		&PlayerOld::pause
	);

	QObject::connect(
		m_player,
		&PlayerOld::pushFrame,
		m_mainWindow,
		&MainWindow::setFrameIndex
	);

	QObject::connect(
		m_mainWindow,
		&MainWindow::itemChanged,
		m_player,
		&PlayerOld::toFrame
	);

	QObject::connect(
		m_annovis,
		&AnnoVis::sendImage,
		m_mainWindow,
		&MainWindow::showPicture
	);

	// done till here

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
		m_mainWindow,
		&MainWindow::sendRightClick,
		m_annovis,
		&AnnoVis::changeSingleEmbryoConfidence
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
		&MainWindow::sendConfident,
		m_annovis,
		&AnnoVis::setConfident
	);

	QObject::connect(
		m_annovis,
		&AnnoVis::sendCounters,
		m_mainWindow,
		&MainWindow::recieveCounters
	);


	QObject::connect(
		m_mainWindow,
		&MainWindow::sendRotationsendRotation,
		m_annovis,
		&AnnoVis::getRotation
	);

}
