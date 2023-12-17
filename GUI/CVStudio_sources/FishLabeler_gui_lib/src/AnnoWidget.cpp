//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "AnnoWidget.h"

#include <QResizeEvent>
#include <qsizepolicy.h>

AnnoWidget::AnnoWidget(QWidget* parent) : QLabel(parent)
{
	this->setScaledContents(true);

	const int minimumHeight = 600;
	const int minimumWidth = 600;

	const int defaultWidth = 600;
	const int defaultHeight = 600;

	this->setMinimumHeight(minimumHeight);
	this->setMinimumWidth(minimumWidth);
	this->resize(defaultWidth, defaultHeight);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

AnnoWidget::~AnnoWidget() {}

void AnnoWidget::resizeEvent(QResizeEvent* event)
{
	QSize size = event->size();
	QSize correctSize = QSize(std::max(size.width(), size.height()), std::max(size.width(), size.height())); // this one causes that behaviour .
	this->resize(correctSize);
	this->setImage(m_img);
}

void AnnoWidget::setImage(const QImage& img)
{
	try
	{
	
	m_img = img;
	if (m_img.isNull())
	{
		return;
	}
	QPixmap paint_pixmap = QPixmap::fromImage(m_img).scaled(this->size());
	this->setPixmap(paint_pixmap);
	
	this->repaint();
	}
	catch (std::exception e)
	{
		std::string str = e.what();
	}

 }

void AnnoWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_img.isNull())
	{
		return;
	}
	const QPoint pos = event->pos();
	const QPoint resizedPos = QPoint(pos.x() * (static_cast<double>(m_img.width()) / static_cast<double>(this->width())),
		pos.y()* (static_cast<double>(m_img.height()) / static_cast<double>(this->height())));
	if (event->button() == Qt::LeftButton) {
		emit sendLeftClick(resizedPos);
	}

	if (event->button() == Qt::MiddleButton) {
		emit wheelPress(resizedPos);
	}
	if (event->button() == Qt::RightButton) {
		emit sendRightClick(resizedPos);
	}

}



void AnnoWidget::wheelEvent(QWheelEvent* event)
{
	if (m_img.isNull())
	{
		return;
	}

	const QPoint pos = event->pos();
	const QPoint resizedPos = QPoint(pos.x() * (static_cast<double>(m_img.width()) / static_cast<double>(this->width())),
		pos.y()* (static_cast<double>(m_img.height()) / static_cast<double>(this->height())));
	emit sendWheelRotation(resizedPos, event->angleDelta() / 8 / 3);
}


/*
void AnnoWidget::keyPressEvent( QKeyEvent* e)
{
	emit(sendKey(e));
}*/