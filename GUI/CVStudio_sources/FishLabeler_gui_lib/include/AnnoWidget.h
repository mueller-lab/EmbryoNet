//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================


#pragma once

#include <QLabel>
#include <QWheelEvent>


class AnnoWidget: public QLabel
{

    Q_OBJECT

public:

    AnnoWidget(QWidget* parent = nullptr);
    ~AnnoWidget();

    virtual void resizeEvent(QResizeEvent*) override;

    void setImage(const QImage& img);

    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    // void keyPressEvent( QKeyEvent* e) override;
    
signals:
    void sendLeftClick(const QPoint& p);
    void sendWheelRotation(const QPoint& pos, const QPoint angleDelta);
    void wheelPress(const QPoint& p);
    void sendRightClick(const QPoint& p);
   // void sendKey( const QKeyEvent *p);



private:

    QImage m_img;




};
