//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include <QShortcut>
#include "MainWindow.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <boost/dll/runtime_symbol_info.hpp>
#include <qkeysequenceedit.h>
#include <QKeySequence>
#include <qapplication.h>


#define TEST_MODE 0

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	allocate();
	connectInternal();
	draw();
}

MainWindow::~MainWindow() {}

void MainWindow::allocate()
{
	
	m_annotationTabWidget = new QTabWidget();

	m_select_all_shortcut = new QShortcut(QKeySequence("Ctrl+A"), this);;
	m_unselect_all_shortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);;
	m_highlight_unknown_shortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);;

	m_save_shortcut = new QShortcut(QKeySequence("Ctrl+S"), this);;
	m_open_shortcut = new QShortcut(QKeySequence("Ctrl+O"), this);;

	m_mainLayout = new QGridLayout;
	m_comboboxLayout = new QHBoxLayout;
	m_saveLoadLayout = new QHBoxLayout;
	m_sliderLayout = new QHBoxLayout;
	m_startStopPauseLayout = new QHBoxLayout;
	m_startButton = new QPushButton("Start");
	m_stopButton = new QPushButton("Stop");
	m_nextFrameButton = new QPushButton(">>");
	m_prevFrameButton = new QPushButton("<<");
	m_saveButton = new QPushButton("Save");
	m_loadButton = new QPushButton("Load");
	m_licenseButton = new QPushButton("Licenses");

	m_comboBox = new QComboBox;
	m_comboBox->setDisabled(true);
	m_startSliderLayout = new QVBoxLayout;
	m_imageList = new QListWidget;
	m_playSlider = new QSlider(Qt::Horizontal);
	m_playTimer = new QTimer;
	m_picture = new AnnoWidget;
	m_confident = new QLabel("CONFIDENT");
	m_helpButton = new QPushButton("Help");
	m_helpLayout = new QVBoxLayout();
	m_embryoCounter = new EmbryoCounter();
	m_rotationWidget = new RotationWidget();
	m_rightLayout = new QVBoxLayout();


#if TEST_MODE
	m_prevFrameButton->setDisabled(true);
	m_imageList->setDisabled(true);
	m_playSlider->setDisabled(true);

#endif // TEST_MODE

}

void MainWindow::draw()
{

	//m_picture->setPixmap(QPixmap("intro.jpg").scaled(QSize(500, 500)));
	m_mainLayout->addWidget(m_picture, 0, 0);
	m_rightLayout->addWidget(m_imageList);
	m_rightLayout->addWidget(m_annotationTabWidget);
	m_annotationTabWidget->addTab(m_embryoCounter, QString("classes"));
	m_annotationTabWidget->addTab(m_rotationWidget, QString("rotations"));

	//m_rightLayout->addWidget(m_embryoCounter);
	m_mainLayout->addLayout(m_rightLayout, 0, 1);
	m_sliderLayout->addWidget(m_comboBox);
	m_sliderLayout->addWidget(m_confident);
	m_sliderLayout->addWidget(m_playSlider);

	m_startStopPauseLayout->addWidget(m_prevFrameButton);
	m_startStopPauseLayout->addWidget(m_stopButton);
	m_startStopPauseLayout->addWidget(m_startButton);
	m_startStopPauseLayout->addWidget(m_nextFrameButton);

	m_startSliderLayout->addLayout(m_sliderLayout);
	m_startSliderLayout->addLayout(m_startStopPauseLayout);

	m_mainLayout->addLayout(m_startSliderLayout, 1, 0);

	m_saveLoadLayout->addWidget(m_saveButton);
	m_saveLoadLayout->addWidget(m_loadButton);
	m_helpLayout->addLayout(m_saveLoadLayout);
	m_helpLayout->addWidget(m_licenseButton);
	m_helpLayout->addWidget(m_helpButton);

	m_mainLayout->addLayout(m_helpLayout, 1, 1);

	this->setLayout(m_mainLayout);

}

void MainWindow::connectInternal()
{
	QObject::connect(
		this->m_select_all_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::selectAll
	);

	QObject::connect(
		this->m_unselect_all_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::unselectAll
	); QObject::connect(
		this->m_highlight_unknown_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::highlightUnknowns
	);


	QObject::connect(
		this->m_save_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::saveRequest
	);

	QObject::connect(
		this->m_open_shortcut,
		&QShortcut::activated,
		this,
		&MainWindow::choseDirectory
	);


	QObject::connect
	(
		this->m_loadButton,
		&QPushButton::clicked,
		this,
		&MainWindow::choseDirectory
	);

	QObject::connect
	(
		this->m_embryoCounter,
		&EmbryoCounter::sendClassId,
		this,
		&MainWindow::sendClass
	);



	QObject::connect
	(
		this->m_startButton,
		&QPushButton::clicked,
		this,
		&MainWindow::startButtonClicked
	);

	QObject::connect
	(
		this->m_stopButton,
		&QPushButton::clicked,
		this,
		&MainWindow::stopButtonClicked
	);

	QObject::connect
	(
		this->m_playSlider,
		&QSlider::valueChanged,
		this,
		&MainWindow::itemChanged
	);

	QObject::connect
	(
		this->m_imageList,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::itemChanged
	);

	QObject::connect
	(
		this->m_picture,
		&AnnoWidget::sendLeftClick,
		this,
		&MainWindow::sendLeftClick
	);

	QObject::connect
	(
		this->m_picture,
		&AnnoWidget::wheelPress,
		this,
		&MainWindow::wheelPress
	);


	QObject::connect
	(
		this->m_picture,
		&AnnoWidget::sendWheelRotation,
		this,
		&MainWindow::sendWheelRotation
	);

	QObject::connect
	(
		this->m_picture,
		&AnnoWidget::sendRightClick,
		this,
		&MainWindow::sendRightClick
	);


	QObject::connect
	(
		this,
		&MainWindow::sendClass,
		this->m_comboBox,
		&QComboBox::setCurrentIndex
	);

	QObject::connect
	(
		this->m_nextFrameButton,
		&QPushButton::clicked,
		[this]() {
		emit itemChanged(std::min(m_imageList->currentRow() + 1, m_imageList->count() - 1));
	}
	);

	QObject::connect
	(
		this->m_prevFrameButton,
		&QPushButton::clicked,
		[this]() {
		emit itemChanged(std::max(0, m_imageList->currentRow() - 1));
	}
	);
	QObject::connect
	(
		this->m_saveButton,
		&QPushButton::clicked,
		this,
		&MainWindow::saveRequest
	);

	QObject::connect
	(
		this->m_helpButton,
		&QPushButton::clicked,
		[]()
	{
		QMessageBox message;
		QString mes = QString("1) Press load and navigate to the experiment folder \n") +
			QString("2) Choose the labeling you want to edit \n") +
			QString("3) Press left click to select one or many bounding boxes \n") +
			QString("4) Press ctrl + number button (1-0) to set selected boxes to classes, according to the loaded class table\n") +
			QString("5) Press alt to set selected tracks to <<Confident>> state \n") +
			QString("6) Press shift to set selected tracks to <<Not Sure >> state \n") +
			QString("7) Use right mouse click to change the single embryo state between <<Confident>> and <<Not Sure>> .\n") +
			QString("8) Use F1-F5 to set the severity for chosen embryos. (0-25-50-75-100)\n") +
			QString("9) Use wheel scroll to adjust severity (step = 5%).\n") +
			QString("10) Press wheel to setup the embryo to <Severe Confident State>.  \n") +
			QString("11) Press Escape to clean selected track.\n") +
			QString("12) Press <Save> button and enter labeling name.\n") +
			QString("13) Labeling will be saved in the experiment folder\n") +
			QString("Default Keys: Bmp (1,B), noDal(2,D), Normal(3,N), Unknown(U,4), booM(5,M), cuT(6,T)\n") +
			QString("Default Keys: Wnt (7,W), FgF(8,F), Shh(9,S), pCp(0,C), rA(-,A)\n") +
			QString("Press \"R\" to change embryo rotation State \n");


		message.setText(mes);
		message.exec();   }
	);

	QObject::connect
	(
		this->m_licenseButton,
		&QPushButton::clicked,
		[]()
	{
		QMessageBox message;
		QString mes = QString("<b>Embryo Labeler software</b><br>") +
			QString("Copyright &copy; 2021,<br>") +
			QString("Computer Vision Studio,<br>") +
			QString("Friedrich Miescher Laboratory of the Max Planck Society,<br>") +
			QString("Systems Biology of Development, University of Konstanz,<br>") +
			QString("Safroshkin Matvey, Grigory Arutyunov, Patrick Mueller <br><br>") +

			QString("<b>License:</b><br>") +
			QString("This software is distributed under the terms of the GNU General Public License<br><br>") +

			QString("<b>Technical Support:</b><br>") +
			QString("Email: cvfn.corporative@gmail.com <br>") +
			QString("E-mail: +49 0152 58731037<br>") +
			QString("Hours: 10:00-18:00 CET<br><br>") +
			QString("<b>3rd party libraries:</b><br>") +
			QString("This software uses libraries from Qt under the LGPLv2.1.<br>") +
			QString("This software uses libraries from OpenCV under the 3-clause BSD license.<br>") +
			QString("This software uses libraries from Boost under the Boost Software License v1.0. <br>") +
			QString("This software uses nlohman json library with MIT License");





		message.setText(mes);
		message.exec();   }
	);



	/* QObject::connect(

		 m_picture,
		 &AnnoWidget::sendKey,
		 this,
		 &MainWindow::keyPressEvent
	 );
	 */


}

void MainWindow::choseDirectory()
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

void MainWindow::setStringList(const QStringList& stringList)
{
	this->m_imageList->clear();
	if (!stringList.isEmpty())
	{
		this->m_imageList->addItems(stringList);
		this->m_imageList->setCurrentRow(0);
	}
}

void MainWindow::setSlider(unsigned int length)
{
	this->m_playSlider->setMinimum(0);
	this->m_playSlider->setMaximum(0);
	if (length > 0)
	{
		this->m_playSlider->setMaximum(length - 1);
		this->m_playSlider->setSliderPosition(0);
	}
}

void MainWindow::setFrameIndex(unsigned int frameIndex)
{
	this->m_playSlider->setSliderPosition(frameIndex);
	this->m_imageList->setCurrentRow(frameIndex);
}

void MainWindow::showPicture(const QImage& qimage)
{
	m_picture->setImage(qimage);
}

void MainWindow::recieveClasses(const std::vector <std::pair<QString, QColor>>& embryoClasses)
{
	m_comboBox->clear();


	for (const auto& embryoClass : embryoClasses)
	{
		m_comboBox->addItem(embryoClass.first);
	}
	//m_rightLayout->removeWidget(m_embryoCounter);

	m_embryoCounter->resetCounter(embryoClasses);
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


	case Qt::Key_F1:
		emit sendConcentration(0);
		break;
	case Qt::Key_F2:
		emit sendConcentration(25);
		break;
	case Qt::Key_F3:
		emit sendConcentration(50);
		break;
	case Qt::Key_F4:
		emit sendConcentration(75);
		break;
	case Qt::Key_F5:
		emit sendConcentration(100);
		break;

	case Qt::Key_Left:

		this->m_playSlider->setSliderPosition(m_playSlider->sliderPosition() - 1);
		break;
	case Qt::UpArrow:
		this->m_playSlider->setSliderPosition(m_playSlider->sliderPosition() + 1);
		break;
	case Qt::Key_Right:
		this->m_playSlider->setSliderPosition(m_playSlider->sliderPosition() + 1);
		break;
	case Qt::RightArrow:
		this->m_playSlider->setSliderPosition(m_playSlider->sliderPosition() + 1);
		break;

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




void MainWindow::recieveCounters(const std::vector<int>& counters)
{
	m_embryoCounter->setCounter(counters);

}
