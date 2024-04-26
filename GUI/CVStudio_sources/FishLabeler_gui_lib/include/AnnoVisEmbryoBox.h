#pragma once 

#include <qvector3d.h>
#include <QColor>
#include <qrect.h>

struct EmbryoBox
{
	QRect bbox;
	size_t id;
	QString className;
	bool isClassConfident = false;
	bool isHighlighted = false;
	bool isConcentrationConfident = false;
	bool isRotated = false;
	bool isKeyFrame = false;
	QVector3D rotation = QVector3D(0.0f, 0.0f, 45.0f);
	QColor color;
	float severe = 0;
};
