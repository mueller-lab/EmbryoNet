//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once


#include <QDialog>

class QPushButton;
class QLineEdit;
class QLabel;
class QVBoxLayout;
class QString;
class SaveWidget : public QDialog
{
    Q_OBJECT

    QLabel* m_label = nullptr;
    QVBoxLayout* m_layout = nullptr;
    QLineEdit* m_nameEdit = nullptr;
    QPushButton* m_savePushButton = nullptr;

public:

    SaveWidget(QWidget* parent = nullptr);
    ~SaveWidget();

signals:
    void sendLabelerName (const QString & s ); 

};
