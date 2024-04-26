//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "PlayerOld.h"

PlayerOld::PlayerOld(QObject* parent): QObject(parent)
{
    m_timer = new QTimer(this);

    QObject::connect
    (
        m_timer,
        &QTimer::timeout,
        this,
        &PlayerOld::nextFrame
    );
}


PlayerOld::~PlayerOld()
{
    pause();
}

void PlayerOld::play()
{
    if (m_current == m_length)
    {
        m_current = 0;
        emit pushFrame(m_current);
    }

    m_timer->setSingleShot(false);
    m_timer->start(100 / 1);    
}

void PlayerOld::pause()
{
    m_timer->stop();
}

void PlayerOld::nextFrame()
{
    if (m_current < m_length)
    {
        emit pushFrame(m_current);
        m_current++;
    }
    else
    {
        pause();
    }
}

void PlayerOld::prevFrame()
{
    if (m_current > 0)
    {
        m_current--;
        emit pushFrame(m_current);
    }
}

void PlayerOld::toFrame(const unsigned int& f)
{
    if ((f >= 0) && (f < m_length))
    {
        m_current = f;
    }
    emit pushFrame(m_current);
}

void PlayerOld::reset()
{
    m_current = 0;
    m_length = 0;
}

void PlayerOld::setFramesLength(const unsigned int length)
{
    m_length = length;
}
