//=======================================================================================================================
//Copyright
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include <QApplication>
#include "MainWindow.h"
#include "Deployer.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    Deployer d(&mainWindow);

    mainWindow.show();

    const int exitStatus = app.exec();
    return exitStatus;
}


