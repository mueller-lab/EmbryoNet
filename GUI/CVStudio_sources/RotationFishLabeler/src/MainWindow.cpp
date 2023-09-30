
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	allocate();
	draw();
}

MainWindow::~MainWindow() {}

void MainWindow::allocate()
{
	m_mainMenu_bar = new QMenuBar;
	m_loadSaveMenu = new QMenu("Save/Load");
	
	m_mainLayout = new QHBoxLayout;
	m_leftVertLayout = new QVBoxLayout;
	m_imageList_normal = new QListWidget;
	m_imageList_invert = new QListWidget;
	m_imageList_TabWidget = new QTabWidget;

	m_horizontal_annotationLayout = new QHBoxLayout;
	m_rotationClassLayout = new QVBoxLayout;
	m_embryoCounterWidget = new EmbryoCounter;
	m_rotationWidget = new RotationWidget;
	m_annoWidget = new AnnoWidget;

	m_sliderButton = new QHBoxLayout;
	m_startSliderLayout = new QVBoxLayout;
	m_rightLayout = new QVBoxLayout;


	m_buttonLayout = new QHBoxLayout;
	m_startButton = new QPushButton("~>");
	m_stopButton = new QPushButton("[]");
	m_nextButton = new QPushButton(">");
	m_previousButton = new QPushButton("<");
	m_fistButton = new QPushButton("<<");
	m_lastButton = new QPushButton(">>");

	m_playSlider = new QSlider(Qt::Orientation::Horizontal);
}

void MainWindow::draw()
{
	m_mainMenu_bar->addMenu(m_loadSaveMenu);
	m_mainLayout->setMenuBar(m_mainMenu_bar);
	m_loadSaveMenu->addAction("Save");
	m_loadSaveMenu->addAction("Load");

	m_imageList_TabWidget->addTab(m_imageList_normal, "normal");
	m_imageList_TabWidget->addTab(m_imageList_invert, "invert");

	m_rotationClassLayout = new QVBoxLayout;

	m_rotationClassLayout->addWidget(m_rotationWidget);
	m_rotationClassLayout->addWidget(m_embryoCounterWidget);

	m_horizontal_annotationLayout->addWidget(m_annoWidget);
	m_horizontal_annotationLayout->addLayout(m_rotationClassLayout);
	m_leftVertLayout->addLayout(m_horizontal_annotationLayout);

	m_leftVertLayout->addLayout(m_buttonLayout);

	m_buttonLayout->addWidget(m_fistButton);
	m_buttonLayout->addWidget(m_previousButton);
	m_buttonLayout->addWidget(m_stopButton);
	m_buttonLayout->addWidget(m_startButton);
	m_buttonLayout->addWidget(m_nextButton);
	m_buttonLayout->addWidget(m_lastButton);

	m_leftVertLayout->addWidget(m_playSlider);

	m_mainLayout->addLayout(m_leftVertLayout);
	m_mainLayout->addWidget(m_imageList_TabWidget);
	setLayout(m_mainLayout);

}
