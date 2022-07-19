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


class Loader : public QObject
{
	Q_OBJECT



public:

	Loader(QObject* parent = nullptr);
	~Loader();

public slots:

	void load(const QString& loader);
	void loadFolder(const QString& FolderPath);

signals:

	void sendLoadFinished();
	void sendProgressPercent(const int& progress);
	void sendPathes(const QStringList& stringList);
	void sendExperiments(const QStringList& experimentList);

	void sendError(const QString& message);
};
