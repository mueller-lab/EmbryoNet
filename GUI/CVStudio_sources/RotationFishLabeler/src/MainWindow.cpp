
#include "MainWindow.h"
#include <QFileDialog>
#include <qshortcut.h>
#include <boost/dll/runtime_symbol_info.hpp>
#include <QSpacerItem>
#include <qvector3d.h>
MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	allocate();
	draw();
	connectInternal();
}

MainWindow::~MainWindow() {}

void MainWindow::allocate()
{


	m_player = new Player;
	m_mainMenu_bar = new QMenuBar;
	m_loadSaveMenu = new QMenu("Load/Save");
	
	//m_mainLayout = new QGridLayout;

	m_mainLayout = new QHBoxLayout;
	m_leftVertLayout = new QVBoxLayout;
	m_imageList_normal = new QListWidget;
	m_imageList_invert = new QListWidget;
	m_imageList_TabWidget = new QTabWidget;

	m_horizontal_annotationLayout = new QHBoxLayout;

	m_rotationClassWidget = new RotationClassWidget;

	m_annoWidget = new AnnoWidget;
	m_annoRCsplitter = new QSplitter(Qt::Orientation::Horizontal);

	m_sliderButton = new QHBoxLayout;
	m_startSliderLayout = new QVBoxLayout;
	m_rightLayout = new QVBoxLayout;

	m_select_all_shortcut = new QShortcut(QKeySequence("Ctrl+A"), this);
	m_unselect_all_shortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
	m_highlight_unknown_shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
	m_save_shortcut = new  QShortcut(QKeySequence("Ctrl+S"), this);
	m_open_shortcut = new  QShortcut(QKeySequence("Ctrl+O"), this);

}


void MainWindow::keyPressEvent(QKeyEvent* e)
{

	int key = e->key();

	switch (e->key()) {
	case Qt::Key_Escape:
		emit sendEscape();
		break;

	case Qt::Key_1:
		emit sendClass(0); //BMP (B)
		break;
	case Qt::Key_B:
		emit sendClass(0); //BMP (B)
		break;

	case Qt::Key_2:
		emit sendClass(1);  //NODAL(D)
		break;
	case Qt::Key_D:
		emit sendClass(1);  //NODAL(D)
		break;

	case Qt::Key_3:
		emit sendClass(2);  //NORMAL(N)
		break;
	case Qt::Key_N:
		emit sendClass(2);  //NORMAL(N)
		break;

	case Qt::Key_4:
		emit sendClass(3);  //UNKNOWN (U)
		break;
	case Qt::Key_U:
		emit sendClass(3);  //UNKNOWN (U)
		break;


	case Qt::Key_5:
		emit sendClass(4);  //BOOM (M)
		break;
	case Qt::Key_M:
		emit sendClass(4);  //BOOM (M)
		break;


	case Qt::Key_6:
		emit sendClass(5);  //CUT (T)
		break;
	case Qt::Key_T:
		emit sendClass(5);  //CUT (T)
		break;


	case Qt::Key_7:
		emit sendClass(6);  //WNT (W)
		break;
	case Qt::Key_W:
		emit sendClass(6);  //WNT (W)
		break;

	case Qt::Key_8:
		emit sendClass(7); //FGF (F)
		break;
	case Qt::Key_F:
		emit sendClass(7); //FGF (F)
		break;


	case Qt::Key_9:
		emit sendClass(8); //SHH (S)
		break;
		//case Qt::Key_S:
		//    emit sendClass(8); //SHH (S)
		//    break;


	case Qt::Key_0:        //PCP (C)
		emit sendClass(9);
		break;
	case Qt::Key_C:        //PCP (C)
		emit sendClass(9);
		break;


	case Qt::Key_Minus:
		emit sendClass(10); //RA (A)
		break;
		//case Qt::Key_A:
		//    emit sendClass(10); //RA (A)
		//    break;

		//case Qt::Key_R:
		//    emit sendRotation();
		//    break;




	case Qt::Key_A:
		emit selectAll();
		break;
	case QKeySequence::SelectAll:
		emit selectAll();
		break;
		//case 16777249: //hardcoded ctrl+a. ToDo: find another way to call it. 
		//    emit selectAll();
		//    break;

	case Qt::Key_Q: //hardcoded ctrl+a. ToDo: find another way to call it. 
		emit highlightUnknowns();
		break;

	case Qt::Key_Z:
		emit unselectAll();
		break;


	default:
		break;
	}
}

void MainWindow::draw()
{
	m_mainMenu_bar->addMenu(m_loadSaveMenu);
	m_mainLayout->setMenuBar(m_mainMenu_bar);
	m_loadAction = m_loadSaveMenu->addAction("Load");
	m_saveAction = m_loadSaveMenu->addAction("Save");



	m_imageList_TabWidget->addTab(m_imageList_normal, "normal");
	m_imageList_TabWidget->addTab(m_imageList_invert, "invert");


	m_annoRCsplitter->addWidget(m_annoWidget);
	m_annoRCsplitter->addWidget(m_rotationClassWidget);
	m_annoRCsplitter->setStretchFactor(0, 1);
	m_annoRCsplitter->setStretchFactor(1, 1);

	m_horizontal_annotationLayout->addWidget(m_annoRCsplitter);
	

	m_leftVertLayout->addLayout(m_horizontal_annotationLayout);
	m_leftVertLayout->addWidget(m_player);

	
	m_mainLayout->addLayout(m_leftVertLayout);
	m_mainLayout->addWidget(m_imageList_TabWidget);
	m_imageList_TabWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	setLayout(m_mainLayout);
}

void MainWindow::chooseDirectory()
{
	const QString defaultPathFileName = "DefaultPath.txt";
	std::string program_location = boost::dll::program_location().parent_path().string();
	std::replace(program_location.begin(), program_location.end(), '\\', '/');
	QFile defaultConfigPath(QString::fromStdString(program_location) + "/" + defaultPathFileName);
	defaultConfigPath.open(QFile::ReadOnly);
	const QString defaultOpenPath = QString::fromLatin1(defaultConfigPath.readAll());


	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), defaultOpenPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (dir.isEmpty()) {
		return;
	}
	emit sendDirectory(dir);
}

void MainWindow::connectInternal()
{

	QObject::connect(
		m_select_all_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::selectAll
	);

	QObject::connect(
		m_unselect_all_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::unselectAll
	);

	QObject::connect(
		m_highlight_unknown_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::highlightUnknowns
	);

	QObject::connect(
		m_save_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::saveRequest
	);

	QObject::connect(
		m_open_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::chooseDirectory
	);

	QObject::connect
	(
		m_rotationClassWidget->embryoCounter(),
		&EmbryoCounter::sendClassId,
		this,
		&MainWindow::sendClass
	);

	QObject::connect
	(
		m_loadAction,
		&QAction::triggered,
		this,
		&MainWindow::chooseDirectory
	);
	QObject::connect
	(
		m_saveAction,
		&QAction::triggered,
		this,
		&MainWindow::saveRequest
	);

	QObject::connect
	(
		m_imageList_normal,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::updatePosition
	);

	QObject::connect
	(
		m_imageList_invert,
		&QListWidget::currentRowChanged,
		this,
		[this](int i)
		{
			const int normalItemPosition = m_imageList_invert->count() - 1 - i;
			updatePosition(normalItemPosition);
		}
	);

	QObject::connect
	(
		m_player,
		&Player::pushFrame,
		this,
		&MainWindow::updatePosition
	);

	QObject::connect
	(
		m_annoWidget,
		&AnnoWidget::sendLeftClick,
		this,
		&MainWindow::sendLeftClick
	);

	QObject::connect
	(
		m_annoWidget,
		&AnnoWidget::wheelPress,
		this,
		&MainWindow::wheelPress
	);

	QObject::connect
	(
		m_annoWidget,
		&AnnoWidget::sendWheelRotation,
		this,
		&MainWindow::sendWheelRotation
	);

	QObject::connect
	(
		m_annoWidget,
		&AnnoWidget::sendRightClick,
		this,
		&MainWindow::sendRightClick
	);

	QObject::connect
	(
		m_rotationClassWidget,
		&RotationClassWidget::send_rot,
		this,
		&MainWindow::sendRotation
	);

}

void MainWindow::setStringList(const QStringList& stringList)
{
	m_current_idx = -1; 
	QStringList reverserdStringList;
	std::reverse_copy(stringList.begin(), stringList.end(), std::back_inserter(reverserdStringList));

	m_imageList_normal->clear();
	m_imageList_invert->clear();
	if (!stringList.isEmpty())
	{
		m_imageList_normal->addItems(stringList);
		m_imageList_invert->addItems(reverserdStringList);
		m_imageList_normal->setCurrentRow(0);
		m_imageList_invert->setCurrentRow(m_imageList_invert->count() - 1);
	}
	m_player->setFramesLength(stringList.count());
}

void MainWindow::showPicture(const QImage& qimage)
{
	m_annoWidget->setImage(qimage);
}

void MainWindow::updatePosition(const int& idx)
{
	if (m_current_idx == idx)
	{
		return;
	}

	m_current_idx = idx;
	
	m_imageList_normal->setCurrentRow(m_current_idx);

	const int invertItemPosition = m_imageList_invert->count() - 1 - m_current_idx;
	m_imageList_invert->setCurrentRow(invertItemPosition);
	
	m_player->toFrame(m_current_idx);

	emit itemChanged(idx);
}

void MainWindow::recieveClasses(const std::vector <std::pair<QString, QColor>>& embryoClasses)
{
	m_rotationClassWidget->embryoCounter()->resetCounter(embryoClasses);
}


void MainWindow::recieveCounters(const std::vector<int>& counters)
{
	m_rotationClassWidget->embryoCounter()->setCounter(counters);
}

void MainWindow::getRotation(const QVector3D& rot)
{
	m_rotationClassWidget->rotationWidget()->recieveRotation(rot);

}