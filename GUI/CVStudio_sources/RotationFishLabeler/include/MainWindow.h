//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================


#pragma once

#include <QComboBox>
#include <QGridLayout>
#include <QToolButton>
#include <QMenu>
#include <qmenubar.h>
#include <QListWidget>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <qtabwidget.h>
#include <QTimer>
#include <QPoint>
#include "AnnoWidget.h"
#include <QLabel>

#include <RotationWidget.h>
#include <EmbryoCounterWidget.h>


class MainWindow : public QWidget
{
	Q_OBJECT

	QMenuBar* m_mainMenu_bar = nullptr;
	QMenu* m_loadSaveMenu = nullptr;
	QHBoxLayout* m_mainLayout = nullptr;
	QVBoxLayout* m_leftVertLayout = nullptr;
	QListWidget* m_imageList_normal = nullptr;
	QListWidget* m_imageList_invert = nullptr;
	QTabWidget* m_imageList_TabWidget = nullptr;


	QHBoxLayout* m_horizontal_annotationLayout = nullptr;
	QVBoxLayout* m_rotationClassLayout = nullptr;
	EmbryoCounter* m_embryoCounterWidget = nullptr;
	RotationWidget* m_rotationWidget = nullptr;
	AnnoWidget* m_annoWidget = nullptr;


	QHBoxLayout* m_sliderButton = nullptr;
	QVBoxLayout* m_startSliderLayout = nullptr;
	QVBoxLayout* m_rightLayout = nullptr;

	QHBoxLayout* m_buttonLayout = nullptr; //todo REDO 
	QSlider* m_playSlider = nullptr;
	QPushButton* m_startButton = nullptr;
	QPushButton* m_stopButton = nullptr;
	QPushButton* m_nextButton = nullptr;
	QPushButton* m_previousButton = nullptr;
	QPushButton* m_fistButton = nullptr;
	QPushButton* m_lastButton = nullptr;
	
private:

	void allocate();
	void draw();
public:

	MainWindow(QWidget * parent = nullptr); 
	~MainWindow();

};

