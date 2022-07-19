//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "ChooseFolderWidget.h"

#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QMessageBox>

#include "boost/filesystem.hpp"

ChooseFolderWidget::ChooseFolderWidget(QWidget* parent): QDialog(parent)
{
    m_folderComboBox = new QComboBox();;
    m_loadButton = new QPushButton("Load");
    m_buttonLayout = new QHBoxLayout();
    m_mainLayout = new QVBoxLayout();

    m_buttonLayout->addWidget(m_loadButton);
    m_mainLayout->addWidget(m_folderComboBox);
    m_mainLayout->addLayout(m_buttonLayout);

    this->setLayout(m_mainLayout);

    QObject::connect(
        m_loadButton,
        &QPushButton::clicked,
        [this] () 
        {
            emit sendFolder(m_folderComboBox->currentText());
            this->close();
        }
    );

}


ChooseFolderWidget::~ChooseFolderWidget()
{
}

void ChooseFolderWidget::setFolderList(const QStringList& stringList)
{
    m_folderComboBox->clear();

    for (const auto& string: stringList)
    {
        m_folderComboBox->addItem(string);
    }
}

