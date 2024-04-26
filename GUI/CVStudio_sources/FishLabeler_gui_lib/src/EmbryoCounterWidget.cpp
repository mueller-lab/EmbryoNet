//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "EmbryoCounterWidget.h"

#include <QString>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>

EmbryoCounter::EmbryoCounter()
{
    m_mainLayout = new QVBoxLayout();
    this->m_signalMapper = new QSignalMapper(this);
    this->setMinimumSize(10, 10);
    this->setLayout(m_mainLayout);
}

EmbryoCounter::~EmbryoCounter()
{
    m_classNames.~vector();
    m_classCounterLabels.~vector();
    m_classLayouts.~vector();
    m_mainLayout->deleteLater();
    m_signalMapper->deleteLater();
}

void EmbryoCounter::getCounter(const int& index)
{

    emit sendClassId(index);
}

void EmbryoCounter::resetCounter(const std::vector <std::pair<QString, QColor>>& embryoClasses)
{
    this->hide();
    m_signalMapper->disconnect();
    for (size_t i = 0; i < m_classNames.size(); i++)
    {
        m_mainLayout->removeItem(m_mainLayout->itemAt(i));
        delete m_classLayouts[i];
        delete m_classCounterLabels[i];
        delete m_classNames[i];
        delete m_classColors[i];
    }
    m_classLayouts.clear(); 
    m_classCounterLabels.clear();
    m_classNames.clear();
    m_classColors.clear();
    for (size_t i = 0; i < embryoClasses.size(); i++)
    {
        m_classNames.push_back(new QLabel(embryoClasses[i].first));


        QImage buffer = QImage(100, 40, QImage::Format_RGB888);
        QPainter qp(&buffer);
        QPen pen;
        pen.setWidth(40);
        pen.setColor(embryoClasses[i].second);
        qp.setPen(pen);
        qp.drawRect(0, 0, 100, 40);

        QPixmap s = QPixmap::fromImage(buffer);

        QPushButton* label_button = new QPushButton;

        label_button->setFlat(true);
        QIcon icon = QIcon(s);
        label_button->setIcon(icon);
        label_button->setText(QString::fromStdString(std::to_string(i)));
        m_signalMapper->setMapping(label_button, i);

        QObject::connect(
           label_button,
           &QPushButton::clicked,
           m_signalMapper,
           QOverload<>::of(&QSignalMapper::map));
    
        m_classColors.push_back(label_button);

        m_classCounterLabels.push_back(new QLabel(QString::number(0)));
        m_classLayouts.push_back(new QHBoxLayout());
        m_classLayouts[i]->addWidget(m_classColors[i]);
        m_classLayouts[i]->addWidget(m_classNames[i]);
        m_classLayouts[i]->addWidget(m_classCounterLabels[i]);
        m_mainLayout->addLayout(m_classLayouts[i]);
    }
        
     
    unsigned int BUTTONSIZE = 30;
    //TODO REFACTOR
    this->setMinimumHeight(m_classLayouts.size()* BUTTONSIZE);

    this->setLayout(m_mainLayout);
    this->show();

    QObject::connect(m_signalMapper, static_cast<void(QSignalMapper::*)(int)>(&QSignalMapper::mapped), this, &EmbryoCounter::getCounter);

}

void EmbryoCounter::setCounter(const std::vector<int>& counters)
{
    try{
        for (size_t i = 0; i < counters.size(); i++)
        {
            m_classCounterLabels[i]->setText(QString::number(counters[i]));
        }
    }
    catch (const std::exception & s)
    {

        std::string ss = s.what();
    }

}
