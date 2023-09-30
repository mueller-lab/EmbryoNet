//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include "SaveWidget.h"


SaveWidget::SaveWidget(QWidget* parent) : QDialog(parent)
{
    m_layout = new QVBoxLayout;

    m_label = new QLabel("Please enter labeler Name" );
    m_nameEdit = new QLineEdit;
    m_savePushButton = new QPushButton ("Save");
                    
    m_layout->addWidget(m_label);
    m_layout->addWidget(m_nameEdit);
    m_layout->addWidget(m_savePushButton);
    this->setLayout(m_layout);

    QObject::connect(
        m_savePushButton,
        &QPushButton::clicked,
        this,
        [this]() {
        emit sendLabelerName(m_nameEdit->text());
        this->close();
    }
    );


}

SaveWidget::~SaveWidget()
{

}
