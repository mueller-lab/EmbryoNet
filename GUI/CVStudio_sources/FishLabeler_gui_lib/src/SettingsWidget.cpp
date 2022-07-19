//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "SettingsWidget.h"

#include <QFileDialog>
#include <fstream>
#include <QMessageBox>

SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent)
{
	this->allocateWidgets();
	this->construct();
	this->connect();

	this->choosePixelSize();

	this->show();
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::allocateWidgets()
{

	m_sizeRadioButton = new QRadioButton;
	m_micronsRadioButton = new QRadioButton;

	m_saveButton = new QPushButton("Save");
	m_loadButton = new QPushButton("Load");
	m_applyButton = new QPushButton("Apply");
	m_defaultButton = new QPushButton("Default");

	m_stringParserLabel = new QLabel("Microscope pattern");
	m_stringParserEdit = new QLineEdit("LO[0-9]+");

	m_startPointLabel = new QLabel("Start point");
	m_startPointDateTime = new QDateTimeEdit();
	m_startPointDateTime->setDisplayFormat("hh:mm");
	m_stopPointLabel = new QLabel("Stop point");
	m_stopPointDateTime = new QDateTimeEdit();
	m_stopPointDateTime->setMaximumTime(QTime(24,00));
	m_stopPointDateTime->setTime(QTime(23, 59));
	m_stopPointDateTime->setDisplayFormat("hh:mm");

	m_embryoMaxSizeLabel = new QLabel("Max Diameter Size [pixels]");
	m_embryoMaxSizeSpinBox = new QSpinBox();
	m_embryoMaxSizeSpinBox->setMaximum(1000);
	m_embryoMaxSizeSpinBox->setMinimum(0);
	m_embryoMaxSizeSpinBox->setValue(450);
	m_embryoMinSizeLabel = new QLabel("Min Diameter Size [pixels]");
	m_embryoMinSizeSpinBox = new QSpinBox();
	m_embryoMinSizeSpinBox->setMaximum(1000);
	m_embryoMinSizeSpinBox->setMinimum(1);
	m_embryoMinSizeSpinBox->setValue(350);

	m_micronsPerPixelLabel = new QLabel("micrometers per pixel");
	m_micronsPerPixelSpinBox = new QSpinBox();
	m_micronsPerPixelSpinBox->setMinimum(1);
	m_micronsPerPixelSpinBox->setMaximum(100000);


	m_averageEmbryoSizeMicronsLabel = new QLabel("Average Diameter [um]");
	m_averageEmbryoSizeMicronsSpinBox = new QSpinBox();
	m_averageEmbryoSizeMicronsSpinBox->setMaximum(100000);
	m_averageEmbryoSizeMicronsSpinBox->setMinimum(1);
	m_averageEmbryoSizeMicronsSpinBox->setValue(300);
	

	m_embryoStdLabel = new QLabel("Diameter Deviation [%]");
	m_embryoStdSpinBox = new QSpinBox;
	m_embryoStdSpinBox->setMaximum(50);
	m_embryoStdSpinBox->setMinimum(1);
	m_embryoStdSpinBox->setValue(15);

	m_mainLayout = new QVBoxLayout;
	m_micronLayout = new QHBoxLayout;
	m_sizeLayout = new  QHBoxLayout;
	m_startStopLayout = new QHBoxLayout;
	m_parserLayout = new QHBoxLayout;
	m_buttonsLayout = new QHBoxLayout;
	m_applyDefaultLayout = new QHBoxLayout;

}

void SettingsWidget::construct()
{

	m_parserLayout->addWidget(m_stringParserLabel);
	m_parserLayout->addWidget(m_stringParserEdit);

	m_startStopLayout->addWidget(m_startPointLabel);
	m_startStopLayout->addWidget(m_startPointDateTime);
	m_startStopLayout->addWidget(m_stopPointLabel);
	m_startStopLayout->addWidget(m_stopPointDateTime);

	m_sizeLayout->addWidget(m_sizeRadioButton);
	m_sizeLayout->addWidget(m_embryoMinSizeLabel);
	m_sizeLayout->addWidget(m_embryoMinSizeSpinBox);
	m_sizeLayout->addWidget(m_embryoMaxSizeLabel);
	m_sizeLayout->addWidget(m_embryoMaxSizeSpinBox);

	m_micronLayout->addWidget(m_micronsRadioButton);
	m_micronLayout->addWidget(m_micronsPerPixelLabel);
	m_micronLayout->addWidget(m_micronsPerPixelSpinBox);
	m_micronLayout->addWidget(m_averageEmbryoSizeMicronsLabel);
	m_micronLayout->addWidget(m_averageEmbryoSizeMicronsSpinBox);
	m_micronLayout->addWidget(m_embryoStdLabel);
	m_micronLayout->addWidget(m_embryoStdSpinBox);

	m_applyDefaultLayout->addWidget(m_applyButton);
	m_applyDefaultLayout->addWidget(m_defaultButton);

	m_buttonsLayout->addWidget(m_saveButton);
	m_buttonsLayout->addWidget(m_loadButton);

	m_mainLayout->addLayout(m_parserLayout);
	m_mainLayout->addLayout(m_startStopLayout);
	m_mainLayout->addLayout(m_sizeLayout);
	m_mainLayout->addLayout(m_micronLayout);
	m_mainLayout->addLayout(m_buttonsLayout);
	m_mainLayout->addLayout(m_applyDefaultLayout);

	this->setLayout(m_mainLayout);
}

void SettingsWidget::connect()
{
	QObject::connect(
		m_sizeRadioButton,
		&QRadioButton::clicked,
		this,
		&SettingsWidget::choosePixelSize
	);

	QObject::connect(
		m_micronsRadioButton,
		&QRadioButton::clicked,
		this,
		&SettingsWidget::chooseMicroSize
	);


	QObject::connect(
		m_loadButton,
		&QPushButton::clicked,
		this,
		&SettingsWidget::loadSettings
	);

	QObject::connect(
		m_saveButton,
		&QPushButton::clicked,
		this,
		&SettingsWidget::saveSettings
	);

	QObject::connect(
		m_defaultButton	,
		&QPushButton::clicked,
		this,
		&SettingsWidget::setDefaultSettings
	);

	QObject::connect(
		m_applyButton,
		&QPushButton::clicked,
		[this]() {
			const nlohmann::json colelctedParams = collectParams();
			const std::string collectedParamsStr = colelctedParams.dump();
			emit this->applySettings(QString::fromStdString(collectedParamsStr));
		}
	);

	connectApplyButton();
	
}

void SettingsWidget::choosePixelSize()
{
	m_sizeRadioButton->setChecked(true);

	m_sizeRadioButton->setDisabled(false);
	m_embryoMinSizeLabel->setDisabled(false);
	m_embryoMinSizeSpinBox->setDisabled(false);
	m_embryoMaxSizeLabel->setDisabled(false);
	m_embryoMaxSizeSpinBox->setDisabled(false);

	m_micronsPerPixelLabel->setDisabled(true);
	m_micronsPerPixelSpinBox->setDisabled(true);
	m_averageEmbryoSizeMicronsLabel->setDisabled(true);
	m_averageEmbryoSizeMicronsSpinBox->setDisabled(true);
	m_embryoStdLabel->setDisabled(true);
	m_embryoStdSpinBox->setDisabled(true);
}

void SettingsWidget::chooseMicroSize()
{
	m_micronsRadioButton->setChecked(true);

	m_embryoMinSizeLabel->setDisabled(true);
	m_embryoMinSizeSpinBox->setDisabled(true);
	m_embryoMaxSizeLabel->setDisabled(true);
	m_embryoMaxSizeSpinBox->setDisabled(true);
	m_micronsPerPixelLabel->setDisabled(false);
	m_micronsPerPixelSpinBox->setDisabled(false);
	m_averageEmbryoSizeMicronsLabel->setDisabled(false);
	m_averageEmbryoSizeMicronsSpinBox->setDisabled(false);
	m_embryoStdLabel->setDisabled(false);
	m_embryoStdSpinBox->setDisabled(false);

}

nlohmann::json SettingsWidget::collectParams()
{
	nlohmann::json output;

	output["MicroscopePattern"] = m_stringParserEdit->text().toStdString();
	output["EmbryoMinSize"] = m_embryoMinSizeSpinBox->value();
	output["EmbryoMaxSize"] = m_embryoMaxSizeSpinBox->value();
	output["StartPoint"] = m_startPointDateTime->text().toStdString();
	output["StopPoint"] = m_stopPointDateTime->text().toStdString();
	output["MicrometersPerPixel"] = m_micronsPerPixelSpinBox->value();
	output["AverageDiameter"] = m_averageEmbryoSizeMicronsSpinBox->value();
	output["DiameterDeviation"] = m_embryoStdSpinBox->value();
	output["MinMaxSize"] = m_sizeRadioButton->isChecked();
	return output;
}

void SettingsWidget::saveSettings()
{
	QString saveFileDir = QFileDialog::getExistingDirectory(this, tr("Save Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (saveFileDir.isEmpty()) {
		return;
	}

	const std::string paramFileName("FishLabelerParams.json");

	nlohmann::json paramsJson = collectParams();
	std::ofstream outputFile(saveFileDir.toStdString() + "/" + paramFileName);
	outputFile << paramsJson.dump(4);

}


void SettingsWidget::loadSettings()
{
	QString saveFileDir = QFileDialog::getOpenFileName(this, tr("Setting json file"), QDir::currentPath(), tr("*.json"));

	if (saveFileDir.isEmpty()) {
		return;
	}

	std::ifstream outputFile(saveFileDir.toStdString());

	nlohmann::json paramsJson;

	outputFile >> paramsJson;
	
	try
	{
		m_stringParserEdit->setText(QString::fromStdString(paramsJson["MicroscopePattern"].get<std::string>()));
		m_embryoMinSizeSpinBox->setValue(paramsJson["EmbryoMinSize"].get<int>());
		m_embryoMaxSizeSpinBox->setValue(paramsJson["EmbryoMaxSize"].get<int>());

		const auto startPoint = QDateTime::fromString(QString::fromStdString(paramsJson["StartPoint"].get<std::string>()), "hh:mm");
		m_startPointDateTime->setDateTime(startPoint);

		const auto stopPoint = QDateTime::fromString(QString::fromStdString(paramsJson["StopPoint"].get<std::string>()), "hh:mm");
		m_stopPointDateTime->setDateTime(stopPoint);


		m_micronsPerPixelSpinBox->setValue(paramsJson["MicrometersPerPixel"].get<int>());
		m_averageEmbryoSizeMicronsSpinBox->setValue(paramsJson["AverageDiameter"].get<int>());
		m_embryoStdSpinBox->setValue(paramsJson["DiameterDeviation"].get<int>());

		if (paramsJson["MinMaxSize"].get<bool>())
		{
			choosePixelSize();
		}
		else
		{
			chooseMicroSize();
		}
	}
	catch (const nlohmann::json::exception &)
	{
		this->showQMessageBox(QString("Not valid json file.\nLoading default parameters."));
		this->setDefaultSettings();
	}
}

void SettingsWidget::showQMessageBox(const QString& s)
{
	QMessageBox b;
	b.setText(s);
	b.exec();
}

void SettingsWidget::setDefaultSettings()
{

	m_stringParserEdit->setText("LO[0-9]+");
	m_embryoMinSizeSpinBox->setValue(0);
	m_embryoMaxSizeSpinBox->setValue(0);

	const auto startPoint = QDateTime::fromString(QString("00:00"), "hh:mm");
	m_startPointDateTime->setDateTime(startPoint);

	const auto stopPoint = QDateTime::fromString(QString("23:59"), "hh:mm");
	m_stopPointDateTime->setDateTime(stopPoint);

	m_micronsPerPixelSpinBox->setValue(0);
	m_averageEmbryoSizeMicronsSpinBox->setValue(0);
	m_embryoStdSpinBox->setValue(0);
	choosePixelSize();

}

void SettingsWidget::connectApplyButton()
{
	QObject::connect(
		m_stringParserEdit,
		&QLineEdit::textChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_sizeRadioButton,
		&QRadioButton::clicked,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_micronsRadioButton,
		&QRadioButton::clicked,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_startPointDateTime,
		&QDateTimeEdit::timeChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_stopPointDateTime,
		&QDateTimeEdit::timeChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_embryoMaxSizeSpinBox,
		&QSpinBox::textChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_embryoMinSizeSpinBox,
		&QSpinBox::textChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_micronsPerPixelSpinBox,
		&QSpinBox::textChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_averageEmbryoSizeMicronsSpinBox,
		&QSpinBox::textChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

	QObject::connect(
		m_embryoStdSpinBox,
		&QSpinBox::textChanged,
		[this]() {
			m_applyButton->setEnabled(true);
		}
	);

}

void SettingsWidget::userSettingsApplied()
{
	m_applyButton->setDisabled(true);
}



