//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once 

#include <QObject>
#include <QTimer>

class Player: public QObject
{
    Q_OBJECT

public:

    Player(QObject* parent = nullptr);
    ~Player();
    
    
public slots:

    void play();
    void pause();
    void nextFrame();
    void prevFrame();
    void toFrame(const unsigned int & f) ;
    void reset();
    void setFramesLength(const unsigned int length);

signals:

    void pushFrame(const unsigned int);

private:

    QTimer*  m_timer = nullptr;
    unsigned int m_length = 0;
    unsigned int m_current = 0;

};
