//=======================================================================================================================
//Copyright
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once

#include <QObject>

#include "Loader.h"
#include "MainWindow.h"
#include "Player.h"
#include "AnnoVis.h"
#include "ChooseFolderWidget.h"

class Deployer : public QObject
{
    Q_OBJECT

private:

    void connect();
    void construct();

public:

    Deployer(MainWindow* mainWindow, QObject* parent = nullptr);
    ~Deployer();

private:

    Loader* m_loader = nullptr;
    MainWindow* m_mainWindow = nullptr;
    Player* m_player = nullptr;
    AnnoVis* m_annovis = nullptr;
    ChooseFolderWidget* m_chooseFolderWidget = nullptr;

};
