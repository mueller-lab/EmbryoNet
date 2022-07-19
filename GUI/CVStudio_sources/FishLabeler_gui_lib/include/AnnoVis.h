//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#pragma once 

#include "nlohmann/json.h"

#include <QColor>
#include <QRect>

#include <QObject>

#define ENABLE_SWITCH_LOGIC 0

class AnnoVis : public QObject
{
	Q_OBJECT

private:

	struct EmbryoBox
	{
		QRect bbox;
		size_t id;
		QString className;
		bool isClassConfident = false;
		bool isHighlighted = false;
		bool isConcentrationConfident = false;
		bool isRotated = false;
		QColor color;
		float severe = 0;
	};

private:

	std::vector <std::vector <EmbryoBox>> m_imagesEmbryoBoxes;
	std::vector<std::pair<std::string, nlohmann::json> > m_images2JSONs;
	size_t m_currentIndex = 0;

	std::map <QString, std::vector<QString> > m_possibleTransitions;

	std::map <QString, QColor> m_classColor;
	std::map <int, QString> m_classIDName;
	std::map < QString, int > m_classNameID;

	void readImageEmbryoBoxes(const size_t imageIdx, const nlohmann::json& imageJson);

	bool isPossible(const QString oldClass, const QString newClass);

	void createPossibleTransitions();

	std::vector <std::pair<std::string, nlohmann::json>> formNewJson();

	int findClosestEmbryoIndex(const QPoint& p);

public:

	AnnoVis(QObject* parent = nullptr);
	~AnnoVis();

	void addItem(const std::string& imagePath, const std::string& json);

	void clear();
	void redrawLast();

public slots:


	void redraw(const size_t& imageIndx);

	void calculateColors(const size_t& imageIndx);
	void calculateColors();
	void selectAll();
	void unselectAll();
	void highlightUnknowns();
	void setImages2JSONs(const std::vector<std::pair<std::string, nlohmann::json> >& images2JSONs);
	void setHighlighted(const QPoint& p);
	void getWheelRotation(const QPoint& pos, const QPoint& angle);
	void wheelPress(const QPoint& pos);
	void changeSingleEmbryoConfidence(const QPoint& pos);

	void setConcentration(const int& concentration);
	void setNewClass(const int& index);
	void setConfident(const bool& confident);
	void clearTracks();
	void processSaveRequest();
	void getRotation();

signals:

	void sendImage(const QImage& img);
	void save(const std::vector<std::pair<std::string, nlohmann::json> >& images2JSONs);
	void sendCounters(const std::vector <int>& counters);
};
