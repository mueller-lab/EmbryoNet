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

class QVBoxLayout;
class QHBoxLayout;
class QString;

class EmbryoCounter : public QWidget
{
    Q_OBJECT
    std::vector<QLabel*> m_classNames;
    std::vector<QLabel*> m_classCounterLabels;
    std::vector<QPushButton*> m_classColors;
    std::vector<QHBoxLayout*> m_classLayouts;
    std::vector<QPixmap*> m_pixmaps;

    QSignalMapper* m_signalMapper = nullptr;
    QVBoxLayout* m_mainLayout;
public:
    EmbryoCounter();
    ~EmbryoCounter();
    void resetCounter(const std::vector <std::pair<QString, QColor>>& embryoClasses);
public slots:

    void setCounter(const std::vector<int>& counters);
    void getCounter(const int& classindx);

signals:
    void sendClassId(const int& classIndx);



};

