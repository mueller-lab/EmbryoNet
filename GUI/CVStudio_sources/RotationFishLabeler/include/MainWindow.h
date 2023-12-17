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
#include <Player.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <qtabwidget.h>
#include <QTimer>
#include <QPoint>
#include "AnnoWidget.h"
#include <QLabel>
#include <qstring.h>
#include <RotationClassWidget.h>
#include <qsplitter.h>



class MainWindow : public QWidget
{
	Q_OBJECT

	Player* m_player = nullptr;
	QMenuBar* m_mainMenu_bar = nullptr;
	QMenu* m_loadSaveMenu = nullptr;
	


	QHBoxLayout* m_mainLayout = nullptr;
	QVBoxLayout* m_leftVertLayout = nullptr;
	QListWidget* m_imageList_normal = nullptr;
	QListWidget* m_imageList_invert = nullptr;
	QTabWidget* m_imageList_TabWidget = nullptr;
	QAction* m_loadAction = nullptr;
	QAction* m_saveAction = nullptr;

	QHBoxLayout* m_horizontal_annotationLayout = nullptr;
	AnnoWidget* m_annoWidget = nullptr;
	QSplitter* m_annoRCsplitter = nullptr;

	QHBoxLayout* m_sliderButton = nullptr;
	QVBoxLayout* m_startSliderLayout = nullptr;
	QVBoxLayout* m_rightLayout = nullptr;

	QHBoxLayout* m_buttonLayout = nullptr; //todo REDO 
	
	QShortcut* m_select_all_shortcut = nullptr;
	QShortcut* m_unselect_all_shortcut = nullptr;
	QShortcut* m_highlight_unknown_shortcut = nullptr;
	QShortcut*  m_save_shortcut = nullptr;
	QShortcut* m_open_shortcut = nullptr;

	RotationClassWidget* m_rotationClassWidget = nullptr;

	int m_current_idx = -1;

private:
	void keyPressEvent(QKeyEvent* e);

	void allocate();
	void draw();
	void connectInternal();

	void chooseDirectory();

public:

	MainWindow(QWidget * parent = nullptr); 
	~MainWindow();

public slots:

	void setStringList(const QStringList& stringList);
	void showPicture(const QImage& image);
	void updatePosition(const int& idx);
	void recieveClasses(const std::vector <std::pair<QString, QColor>>&);
	void recieveCounters(const std::vector <int>& counters);

	void getRotation(const QVector3D& rot);

signals:
	
	void sendRotation(const QVector3D& rot);

	void sendLeftClick(const QPoint& p);
	void sendClassChanged(const QString& className, const int& index);
	void sendEscape();

	void sendConcentration(const int& i);

	void sendRightClick(const QPoint& p);
	void wheelPress(const QPoint& p);
	void sendWheelRotation(const QPoint& pos, const QPoint& angle);

	void sendDirectory(const QString& pathToDir);
	void itemChanged(int i);
	void sendClass(const int&);

	void saveRequest();
	void selectAll();
	void unselectAll();
	void highlightUnknowns();

};

