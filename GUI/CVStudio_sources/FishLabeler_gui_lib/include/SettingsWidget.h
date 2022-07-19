//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once
#include "nlohmann/json.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>


class SettingsWidget: public QWidget
{
	Q_OBJECT

	QPushButton* m_saveButton = nullptr;
	QPushButton* m_loadButton = nullptr;
	QPushButton* m_applyButton = nullptr;
	QPushButton* m_defaultButton = nullptr;
	QHBoxLayout* m_buttonsLayout = nullptr;

	QRadioButton* m_sizeRadioButton = nullptr;
	QRadioButton* m_micronsRadioButton = nullptr;

	QLabel* m_stringParserLabel = nullptr;
	QLineEdit* m_stringParserEdit = nullptr;
	QHBoxLayout* m_parserLayout = nullptr;

	QLabel* m_startPointLabel = nullptr;
	QDateTimeEdit* m_startPointDateTime = nullptr;
	QLabel* m_stopPointLabel = nullptr;
	QDateTimeEdit* m_stopPointDateTime = nullptr;
	QHBoxLayout* m_startStopLayout = nullptr;


	QLabel* m_embryoMaxSizeLabel = nullptr;
	QSpinBox* m_embryoMaxSizeSpinBox = nullptr;
	QLabel* m_embryoMinSizeLabel = nullptr;
	QSpinBox* m_embryoMinSizeSpinBox = nullptr;
	QHBoxLayout* m_sizeLayout = nullptr;

	QLabel* m_micronsPerPixelLabel = nullptr;
	QSpinBox* m_micronsPerPixelSpinBox = nullptr;
	QHBoxLayout* m_micronLayout = nullptr;

	QLabel* m_averageEmbryoSizeMicronsLabel = nullptr;
	QSpinBox* m_averageEmbryoSizeMicronsSpinBox = nullptr;
	QLabel* m_embryoStdLabel = nullptr;
	QSpinBox* m_embryoStdSpinBox = nullptr;

	QVBoxLayout* m_mainLayout = nullptr;
	
	QHBoxLayout* m_applyDefaultLayout = nullptr;


public: 

	SettingsWidget(QWidget* parent=nullptr);
	~SettingsWidget();


private: 

	void allocateWidgets();
	void construct();

	void connect();
	void choosePixelSize();
	void chooseMicroSize();

	nlohmann::json collectParams();
	
	void loadSettings(); 
	void showQMessageBox(const QString& s);
	void saveSettings();
	void setDefaultSettings();
	void connectApplyButton();
public slots:
	void userSettingsApplied();
signals: 

	void applySettings(const QString& json);
	
};
