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

    QGridLayout* m_mainLayout = nullptr;
    QHBoxLayout* m_comboboxLayout = nullptr;
    QHBoxLayout* m_saveLoadLayout = nullptr;
    QHBoxLayout* m_sliderLayout = nullptr;
    QHBoxLayout* m_startStopPauseLayout = nullptr;
    QHBoxLayout* m_sliderButton = nullptr;
    QVBoxLayout* m_startSliderLayout = nullptr;
    QVBoxLayout* m_rightLayout = nullptr;

    QTabWidget* m_annotationTabWidget = nullptr;

    QVBoxLayout* m_helpLayout = nullptr;
    QPushButton* m_startButton = nullptr;
    QPushButton* m_stopButton = nullptr;
    QPushButton* m_nextFrameButton = nullptr;
    QPushButton* m_prevFrameButton = nullptr;
    QPushButton* m_saveButton = nullptr;
    QPushButton* m_loadButton = nullptr;
    QPushButton* m_helpButton = nullptr;
    QPushButton* m_licenseButton = nullptr;

    QListWidget* m_imageList = nullptr;
    QComboBox* m_comboBox = nullptr;
    QLabel* m_confident = nullptr;

    QSlider* m_playSlider = nullptr;
    QTimer* m_playTimer = nullptr;
    AnnoWidget* m_picture = nullptr;
    EmbryoCounter* m_embryoCounter = nullptr;
    RotationWidget *m_rotationWidget = nullptr;
    QShortcut* m_select_all_shortcut = nullptr;
    QShortcut* m_unselect_all_shortcut = nullptr;
    QShortcut* m_highlight_unknown_shortcut = nullptr;

    QShortcut* m_save_shortcut = nullptr;
    QShortcut* m_open_shortcut = nullptr;

    bool m_isconfident = true;

private:

    void allocate();
    void draw();
    void connectInternal();
    void keyPressEvent( QKeyEvent* e);


private slots:

    void choseDirectory();
    

public:

    MainWindow(QWidget* parent = nullptr);
    
    ~MainWindow();

public slots:
   
    void setStringList(const QStringList& stringList);
    void setSlider(unsigned int length);
    void setFrameIndex(unsigned int frameIndex);
    void showPicture(const QImage& qimage);
    void recieveClasses(const std::vector <std::pair<QString, QColor>>&);
    void recieveCounters(const std::vector <int>& counters);

signals:

    void sendDirectory(const QString& s);
    void startButtonClicked();
    void stopButtonClicked();
    void itemChanged(int i);

    void sendLeftClick(const QPoint& p);

    void sendRightClick(const QPoint& p);
    void wheelPress(const QPoint& p);
    void sendWheelRotation(const QPoint& pos, const QPoint& angle);

    void sendRotationsendRotation();


    void sendClassChanged(const QString& className, const int& index);
    void sendEscape();
    void sendClass(const int &);
    void sendConcentration(const int & i);

    void saveRequest();
    void sendConfident(const bool & confident);

    void selectAll();
    void unselectAll();
    void highlightUnknowns();
};
