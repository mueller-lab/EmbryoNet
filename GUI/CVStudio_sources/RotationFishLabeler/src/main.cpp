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

#include <iostream>

int main(int argc, char **argv)
{
    int exitStatus = -1;
    try
    {
        QApplication app(argc, argv);
        app.setWindowIcon(QIcon("EmbryoLabeler.ico"));
        MainWindow mainWindow;
        Deployer d(&mainWindow);

        mainWindow.show();

        exitStatus = app.exec();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return exitStatus;

}
