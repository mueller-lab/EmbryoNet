//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once 

#include <QWidget>
#include <qpushbutton.h>
#include <QLabel>
#include "qsignalmapper.h"
#include <QLineEdit>
#include <QPushButton>


class QVBoxLayout;
class QHBoxLayout;
class QString;

class RotationWidget : public QWidget
{
    Q_OBJECT


    int x = 0; 
    int y = 0;
    int z = 0;
    
    QVBoxLayout* m_mainLayout = nullptr;
    QHBoxLayout* x_Layout = nullptr;
    QHBoxLayout* y_Layout = nullptr;
    QHBoxLayout* z_Layout = nullptr;
    QLabel* x_label = nullptr;
    QLabel* y_label = nullptr;
    QLabel* z_label = nullptr;

    QLineEdit* m_xLineEdit = nullptr;
    QLineEdit* m_yLineEdit = nullptr;
    QLineEdit* m_zLineEdit = nullptr;


    QPushButton* m_add_x_button = nullptr;
    QPushButton* m_add_y_button = nullptr;
    QPushButton* m_add_z_button = nullptr;
    QPushButton* m_add_5x_button = nullptr;
    QPushButton* m_add_5y_button = nullptr;
    QPushButton* m_add_5z_button = nullptr;

    QPushButton* m_subtract_x_button = nullptr;
    QPushButton* m_subtract_y_button = nullptr;
    QPushButton* m_subtract_z_button = nullptr;
    QPushButton* m_subtract_5x_button = nullptr;
    QPushButton* m_subtract_5y_button = nullptr;
    QPushButton* m_subtract_5z_button = nullptr;
    
public:
    RotationWidget();
    ~RotationWidget();
    void resetRotation();


public slots:
    void set_x(const int& x); 
    void set_y(const int& y);
    void set_z(const int& z);

    void recieveRotation(const QVector3D& rot);

signals:
    void send_rot(const QVector3D& rot );

private:
    void allocate(); 
    void draw();
    void connectInternal();
};

