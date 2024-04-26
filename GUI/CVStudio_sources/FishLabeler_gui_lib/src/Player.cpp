//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "Player.h"

Player::Player(QWidget* parent) : QWidget(parent)
{
	allocate();
	draw();
	connect();




}

void Player::allocate()
{
	m_mainLayout = new QVBoxLayout;
	m_buttonLayout = new QHBoxLayout;
	m_startButton = new QPushButton("~>");
	m_stopButton = new QPushButton("[]");
	m_nextButton = new QPushButton(">");
	m_previousButton = new QPushButton("<");
	m_fistButton = new QPushButton("<<");
	m_lastButton = new QPushButton(">>");

	m_playSlider = new QSlider(Qt::Orientation::Horizontal);


	m_timer = new QTimer(this);
}

void Player::draw()
{
	this->setLayout(m_mainLayout);
	m_buttonLayout->addWidget(m_fistButton);
	m_buttonLayout->addWidget(m_previousButton);
	m_buttonLayout->addWidget(m_stopButton);
	m_buttonLayout->addWidget(m_startButton);
	m_buttonLayout->addWidget(m_nextButton);
	m_buttonLayout->addWidget(m_lastButton);

	m_mainLayout->addLayout(m_buttonLayout);
	m_mainLayout->addWidget(m_playSlider);
}

void Player::connect()
{
	QObject::connect
	(
		m_timer,
		&QTimer::timeout,
		this,
		&Player::nextFrame
	);

	QObject::connect
	(
		m_playSlider,
		&QSlider::valueChanged,
		this,
		&Player::toFrame
	);


	QObject::connect
	(
		m_nextButton,
		&QPushButton::clicked,
		this,
		&Player::nextFrame
	);

	QObject::connect
	(
		m_previousButton,
		&QPushButton::clicked,
		this,
		&Player::prevFrame
	);

	QObject::connect
	(
		m_startButton,
		&QPushButton::clicked,
		this,
		&Player::play
	);

	QObject::connect
	(
		m_stopButton,
		&QPushButton::clicked,
		this,
		&Player::pause
	);

	QObject::connect
	(
		m_fistButton,
		&QPushButton::clicked,
		[this]()
		{
			pause();
			toFrame(0);
		}
	);

	QObject::connect
	(
		m_lastButton,
		&QPushButton::clicked,
		[this]()
		{
			pause();
			toFrame(m_length - 1);
		}
	);
}

Player::~Player()
{
	pause();
}

void Player::play()
{
	if (m_current == m_length)
	{
		m_current = 0;
		emit pushFrame(m_current);
	}

	m_timer->setSingleShot(false);
	m_timer->start(100);
}

void Player::pause()
{
	m_timer->stop();
}

void Player::nextFrame()
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

void Player::prevFrame()
{
	if (m_current > 0)
	{
		m_current--;
		emit pushFrame(m_current);
	}
}

void Player::toFrame(const unsigned int& f)
{
	if ((f >= 0) && (f < m_length))
	{
		m_current = f;
	}

	if (m_playSlider->sliderPosition()!= m_current)
	{ 
		m_playSlider->setSliderPosition(f);
	}
	emit pushFrame(m_current);
}

void Player::reset()
{
	m_current = 0;
	m_length = 0;
}

void Player::setFramesLength(const unsigned int length)
{
	m_length = length;

	m_playSlider->setMinimum(0);
	m_playSlider->setMaximum(0);
	if (length > 0)
	{
		m_playSlider->setMaximum(length - 1);
		m_playSlider->setSliderPosition(0);
	}
}

