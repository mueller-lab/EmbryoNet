#include "RotationClassWidget.h"

RotationClassWidget::RotationClassWidget(QWidget* parent): QWidget(parent)
{
	m_layout = new QVBoxLayout;

	m_embryoCounterWidget = new EmbryoCounter;
	m_rotationWidget = new RotationWidget;

	m_splitter = new QSplitter(Qt::Orientation::Vertical);


	m_annotationRotationSpacerItem = new QSpacerItem(300, 400, QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_splitter->addWidget(m_rotationWidget);
	m_splitter->addWidget(m_embryoCounterWidget);

	m_layout->addWidget(m_splitter);
	m_layout->addSpacerItem(m_annotationRotationSpacerItem);

	m_rotationWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_embryoCounterWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	setLayout(m_layout);

	QObject::connect(m_rotationWidget, &RotationWidget::send_rot, this, &RotationClassWidget::send_rot);
}

RotationClassWidget::~RotationClassWidget() {}

EmbryoCounter* RotationClassWidget::embryoCounter() const
{
	return m_embryoCounterWidget;
}

RotationWidget* RotationClassWidget::rotationWidget() const
{
	return m_rotationWidget;
}
