//=======================================================================================================================
//Copyright
//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================


#include "MainWindow.h"
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QDir>


MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	allocate();
	draw();
	connectInternal();
}

MainWindow::~MainWindow()
{
}

void MainWindow::allocate()
{
	m_settingsWidget = new SettingsWidget;
	m_settingsWidget->hide();

	m_annoWidget = new AnnoWidget;
	m_annoVis = new AnnoVis;

	m_imageList = new QListWidget;
	m_imageNamesLabel = new QLabel("Images:");
	m_experimentNamesLabel = new QLabel("Experiments:");

	m_experimentList = new QListWidget;
	m_embryoCounter = new EmbryoCounter;
	m_progressBarLayout = new QVBoxLayout;

	m_loadStartLayout = new QHBoxLayout;
	m_loadButton = new QPushButton("Load");
	m_configVLayout = new QVBoxLayout;
	m_experimentInfoLayout = new QVBoxLayout;

	//m_configWidget = new ConfigWidget; //to do 
	m_startButton = new QPushButton("Start");
	m_infoHelpButton = new QPushButton("Help");
	m_licenseButton = new QPushButton("Licenses");

	m_settingsButton = new QPushButton("Settings");


	m_mainLayout = new QGridLayout;
	m_rightButtonsLayout = new QVBoxLayout;
	//m_folderPathLabel = new QLabel("Folder: ");
	m_leftButtonLayout = new QVBoxLayout;
	m_experimentImageLayout = new QGridLayout;

	m_progressBarLabel = new QLabel("Loading images");
	m_algoritmProgressBarLabel = new QLabel("Processing images");

	m_loadProgressBar = new QProgressBar;
	m_loadProgressBar->setRange(0, 100);

	m_algoritmProgressBar = new QProgressBar;
	m_algoritmProgressBar->setRange(0, 100);

}

void MainWindow::draw()
{
	m_loadStartLayout->addWidget(m_loadButton);
	m_loadStartLayout->addWidget(m_startButton);

	m_experimentImageLayout->addWidget(m_experimentNamesLabel, 0, 0, 1, -1);
	m_experimentImageLayout->addWidget(m_experimentList, 1, 0, 2, -1);
	m_experimentImageLayout->addWidget(m_imageNamesLabel, 3, 0, 1, -1);
	m_experimentImageLayout->addWidget(m_imageList, 4, 0, -1, -1);

	m_experimentInfoLayout->addLayout(m_experimentImageLayout);
	m_experimentInfoLayout->addLayout(m_configVLayout);

	m_progressBarLayout->addWidget(m_progressBarLabel);
	m_progressBarLayout->addWidget(m_loadProgressBar);

	m_progressBarLayout->addWidget(m_algoritmProgressBarLabel);
	m_progressBarLayout->addWidget(m_algoritmProgressBar);

	m_experimentInfoLayout->addLayout(m_progressBarLayout);
	m_experimentInfoLayout->addWidget(m_embryoCounter);
	m_experimentInfoLayout->addWidget(m_settingsButton);
	m_mainLayout->addWidget(m_annoWidget, 0, 0);
	m_leftButtonLayout->addLayout(m_loadStartLayout);
	m_mainLayout->addLayout(m_leftButtonLayout, 1, 0);

	m_mainLayout->addLayout(m_experimentInfoLayout, 0, 1);

	m_rightButtonsLayout->addWidget(m_infoHelpButton);
	m_rightButtonsLayout->addWidget(m_licenseButton);
	m_mainLayout->addLayout(m_rightButtonsLayout, 1, 1);

	this->setLayout(m_mainLayout);
}

void MainWindow::prepareLoad()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (dir.isEmpty()) {
		return;
	}

	emit sendDirectory(dir);

	//this->blockConnectonsOnLoad();
}

void MainWindow::connectInternal()
{
	QObject::connect
	(
		this->m_loadButton,
		&QPushButton::clicked,
		this,
		&MainWindow::prepareLoad
	);

	QObject::connect
	(
		this->m_imageList,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::itemChanged
	);

	QObject::connect
	(
		this->m_startButton,
		&QPushButton::clicked,
		this,
		&MainWindow::startProcessing
	);

	QObject::connect
	(
		this->m_experimentList,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::experimentChanged
	);

	QObject::connect
	(
		this->m_annoVis,
		&AnnoVis::sendImage,
		this->m_annoWidget,
		&AnnoWidget::setImage
	);

	QObject::connect
	(
		this->m_annoVis,
		&AnnoVis::sendCounters,
		this->m_embryoCounter,
		&EmbryoCounter::setCounter,
		Qt::ConnectionType::DirectConnection
	);

	QObject::connect(
		this->m_settingsButton,
		&QPushButton::clicked,
		[this]() {
			if (this->m_settingsWidget->isHidden())
			{
				this->m_settingsWidget->show();
			}
			else
			{
				this->m_settingsWidget->hide();
			}
		}
	);


	QObject::connect(
		this->m_settingsWidget,
		&SettingsWidget::applySettings,
		this,
		&MainWindow::sendSettings
	);

	QObject::connect(
		this,
		&MainWindow::userSettingsApplied,
		m_settingsWidget,
		&SettingsWidget::userSettingsApplied
	);

	QObject::connect
	(
		this->m_licenseButton,
		&QPushButton::clicked,
		[]()
		{
			QMessageBox message;
			QString mes = QString("<b>Embryo Classifier software</b><br>") +
				QString("Copyright &copy; 2021,<br>") +
				QString("Computer Vision Studio,<br>") +
				QString("Friedrich Miescher Laboratory of the Max Planck Society,<br>") +
				QString("Systems Biology of Development, University of Konstanz,<br>") +
				QString("Safroshkin Matvey, Grigory Arutyunov, Patrick Mueller <br><br>") +

				QString("<b>License:</b><br>") +
				QString("This software is distributed under the terms of the GNU General Public License<br><br>") +

				QString("<b>Technical Support:</b><br>") +
				QString("Email: cvfn.corporative@gmail.com <br>") +
				QString("E-mail: +49 0152 58731037<br>") +
				QString("Hours: 10:00-18:00 CET<br><br>") +
				QString("<b>3rd party libraries:</b><br>") +
				QString("This software uses libraries from Qt under the LGPLv2.1.<br>") +
				QString("This software uses libraries from OpenCV under the 3-clause BSD license.<br>") +
				QString("This software uses libraries from Boost under the Boost Software License v1.0. <br>") +
				QString("This software uses libraries from ONNXRuntime under MIT License.<br>") +
				QString("This software uses ") +
				QString("<a href=\"https://docs.nvidia.com/cuda/eula/index.html\">CUDA.</a> <br>") + 
				QString("This software uses nlohman json library with MIT License");

			message.setText(mes);
			message.exec();   }
	);



}

void MainWindow::startProcessing()
{
	if (m_experimentList->count() == 0)
	{
		return;
	}

	m_runningExperiment = { true };
	int m_currentExperimtentIndex = 0;
	m_experimentList->setCurrentRow(m_currentExperimtentIndex);
	while (m_imageList->count() == 0 && (m_currentExperimtentIndex < m_experimentList->count()))
	{
		m_currentExperimtentIndex++;
		m_experimentList->setCurrentRow(m_currentExperimtentIndex);
	}

	emit startProcess(m_experimentList->item(m_experimentList->currentRow())->text());
	blockButtons();
	blockListConnections();
}

void MainWindow::stopProcessing()
{
	restoreButtons();
	restoreListConnections();

	if (m_currentExperimtentIndex < m_experimentList->count() - 1)
	{
		m_currentExperimtentIndex++;
		m_experimentList->setCurrentRow(m_currentExperimtentIndex);

		emit startProcess(m_experimentList->item(m_experimentList->currentRow())->text());
		blockButtons();
		blockListConnections();
		return;
	}
	m_runningExperiment = false;

	m_currentExperimtentIndex = 0;
}


void MainWindow::updateLoadProgressBar(const int& progress)
{
	m_loadProgressBar->setValue(progress);
}

void MainWindow::updateAlgoritmProgressBar(const int& progress)
{
	m_algoritmProgressBar->setValue(progress);
}

void MainWindow::blockListConnections()
{
	QObject::disconnect
	(
		this->m_experimentList,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::experimentChanged
	);
	QObject::disconnect
	(
		this->m_imageList,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::itemChanged
	);

	blockLists();
}

void MainWindow::restoreListConnections()
{
	QObject::connect
	(
		this->m_experimentList,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::experimentChanged
	);
	QObject::connect
	(
		this->m_imageList,
		&QListWidget::currentRowChanged,
		this,
		&MainWindow::itemChanged
	);

	restoreLists();
}

void MainWindow::blockLists()
{
	m_imageList->setDisabled(true);
	m_experimentList->setDisabled(true);

}

void MainWindow::restoreLists()
{
	m_imageList->setDisabled(false);
	m_experimentList->setDisabled(false);
}

void MainWindow::blockButtons()
{
	m_loadButton->setDisabled(true);
	m_startButton->setDisabled(true);

}

void MainWindow::restoreButtons()
{
	m_loadButton->setDisabled(false);
	m_startButton->setDisabled(false);
}

void MainWindow::blockConnectonsOnLoad()
{
	m_wereListsBlocked = !m_experimentList->isEnabled();

	blockLists();
	blockButtons();
}

void MainWindow::restoreConnectonsOnLoad()
{
	if (!m_wereListsBlocked && !m_runningExperiment)
	{
		restoreLists();
		restoreButtons();
	}
}

void MainWindow::setStringList(const QStringList& stringList)
{
	this->m_imageList->clear();
	if (!stringList.isEmpty())
	{
		this->m_imageList->addItems(stringList);
		this->m_imageList->setCurrentRow(0);
	}
	m_loadedExperiments[m_experimentList->currentItem()->text()] = stringList;
}

void MainWindow::setExperimentStringList(const QStringList& stringList)
{
	m_loadedExperiments.clear();
	this->m_experimentList->clear();
	if (!stringList.isEmpty())
	{
		this->m_experimentList->addItems(stringList);
		this->m_experimentList->setCurrentRow(0);
	}
}

void MainWindow::loadPicture(const QString& s)
{
	QImage img(s);
	m_annoWidget->setImage(img);
}

void MainWindow::itemChanged(const int& currentRow)
{
	if (currentRow >= 0)
	{
		QString pathToImage = m_imageList->item(currentRow)->text();
		loadPicture(pathToImage);
	}
}

void MainWindow::experimentChanged(const int& currentRow)
{
	if (currentRow >= 0)
	{
		m_annoVis->clear();
		QString pathToExperiment = m_experimentList->item(currentRow)->text();
		if (m_loadedExperiments.find(pathToExperiment) == m_loadedExperiments.end())
		{
			emit requestImageList(pathToExperiment);
			blockConnectonsOnLoad();
		}
		else
		{
			setStringList(m_loadedExperiments[pathToExperiment]);
		}
	}
}

void MainWindow::clearAnnovis()
{
	m_annoVis->clear();
}

void MainWindow::showProcessedImage(const std::string& pathToImg, const std::string& json)
{
	//здесь упало но хз как вопсроизвести 
	QList<QListWidgetItem*> currentItemList = m_imageList->findItems(QString::fromStdString(pathToImg), Qt::MatchFixedString);
	if (!currentItemList.empty())
	{
		m_imageList->setCurrentItem(currentItemList.at(0));
		m_annoVis->addItem(pathToImg, json);
		m_annoVis->calculateColors();
		m_annoVis->redrawLast();
	}
}

void MainWindow::recieveClasses(const std::vector <std::pair<QString, QColor>>& embryoClasses)
{
	m_embryoCounter->resetCounter(embryoClasses);
}

void MainWindow::showQMessageBox(const QString& s)
{
	QMessageBox b;
	b.setText(s);
	b.exec();
}

void MainWindow::onError(const QString& s)
{

	showQMessageBox(s);
	m_currentExperimtentIndex = m_experimentList->count();
	this->stopProcessing();


}

void MainWindow::closeEvent(QCloseEvent* event)
{
	m_settingsWidget->close();
}