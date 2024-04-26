//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once 

#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>

class Player : public QWidget
{
    Q_OBJECT

    QVBoxLayout* m_mainLayout = nullptr;
    
    QHBoxLayout* m_buttonLayout = nullptr; 
    QSlider* m_playSlider = nullptr;
    QPushButton* m_startButton = nullptr;
    QPushButton* m_stopButton = nullptr;
    QPushButton* m_nextButton = nullptr;
    QPushButton* m_previousButton = nullptr;
    QPushButton* m_fistButton = nullptr;
    QPushButton* m_lastButton = nullptr;

public:

    Player(QWidget* parent = nullptr);
    ~Player();
    void allocate();
    void draw();
    void connect();

public slots:

    void play();
    void pause();
    void nextFrame();
    void prevFrame();
    void toFrame(const unsigned int& f);
    void reset();
    void setFramesLength(const unsigned int length);

signals:

    void pushFrame(const unsigned int);

private:

    QTimer* m_timer = nullptr;
    unsigned int m_length = 0;
    unsigned int m_current = 0;

};
