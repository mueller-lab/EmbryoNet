//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once 

#include <QDialog>

class QComboBox;
class QPushButton;

class QHBoxLayout;
class QVBoxLayout;

class ChooseFolderWidget: public QDialog
{
    Q_OBJECT

    QComboBox* m_folderComboBox = nullptr;
    QPushButton* m_loadButton = nullptr;
    QHBoxLayout* m_buttonLayout = nullptr;
    QVBoxLayout* m_mainLayout = nullptr;
    
public:

    ChooseFolderWidget(QWidget* parent = nullptr);
    ~ChooseFolderWidget();

    void setFolderList(const QStringList& stringList);

signals:

    void sendFolder(const QString& pathToJson);

};
