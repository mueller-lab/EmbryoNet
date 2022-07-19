//=======================================================================================================================
//Copyright(C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
//Computer Vision Studio
//Friedrich Miescher Laboratory of the Max Planck Society
//Systems Biology of Development, University of Konstanz
//This software is distributed under the terms of the GNU General Public License
//=======================================================================================================================

#include "AnnoVis.h"

#include <QImage>
#include <QPainter>
#include <QPaintDevice>

#include <QMessageBox>

#include <set>
#define TEST_MODE 1
namespace
{

	void drawRect(QImage* qimg, const QRect& rect, const QColor& color, const int penWidth, const bool isConfident)
	{
		QPainter qp(qimg);

		QPen pen;
		pen.setColor(color);
		pen.setWidth(penWidth);
		if (!isConfident)
		{
			pen.setStyle(Qt::DashDotLine);
		}

		qp.setPen(pen);
		qp.drawRect(rect);
	}

	template<typename NumberType>
	void drawText(QImage* qimg, const QRect& rect, const QColor& color, const int penWidth, NumberType id)
	{
		QPainter qp(qimg);

		const int textSize = rect.width() / 5;
		QFont font;
		font.setPixelSize(textSize);

		qp.setFont(font);

		QPen pen;

		pen.setColor(color);
		pen.setWidth(penWidth);

		qp.setPen(pen);

		qp.drawText(rect.center(), QString::number(id));
	}

	template<>
	void drawText(QImage* qimg, const QRect& rect, const QColor& color, const int penWidth, QString text)
	{
		QPainter qp(qimg);

		const int textSize = rect.width() / 5;
		QFont font;
		font.setPixelSize(textSize);

		qp.setFont(font);

		QPen pen;

		pen.setColor(color);
		pen.setWidth(penWidth);

		qp.setPen(pen);

		qp.drawText(rect.center(), text);
	}

} // namespace

AnnoVis::AnnoVis(QObject* parent) : QObject(parent)
{
	createPossibleTransitions();
}

AnnoVis::~AnnoVis() {
}

void AnnoVis::clear()
{
	m_imagesEmbryoBoxes.clear();
	m_images2JSONs.clear();
	m_currentIndex = 0;
	m_possibleTransitions.clear();
	m_classColor.clear();
	m_classIDName.clear();
	m_classNameID.clear();
}

void AnnoVis::addItem(const std::string& imagePath, const std::string& json)
{
	m_images2JSONs.push_back({ imagePath, nlohmann::json::parse(json) });

	m_imagesEmbryoBoxes.resize(m_images2JSONs.size());

	this->readImageEmbryoBoxes(m_images2JSONs.size() - 1, nlohmann::json::parse(json));
}

void AnnoVis::readImageEmbryoBoxes(const size_t imageIdx, const nlohmann::json& imageJson)
{
	const nlohmann::json& detectionClasses = m_images2JSONs[imageIdx].second["classes"];
	for (const auto& embryoClass : detectionClasses)
	{
		const std::string className = embryoClass["name"];
		int index = static_cast<int> (embryoClass["id"]);

		m_classIDName[index] = QString::fromStdString(className);
		m_classNameID[QString::fromStdString(className)] = index;

		std::vector <int> colors;
		for (const auto& colorJSON : embryoClass["color"])
		{
			colors.push_back(static_cast<int>(colorJSON));
		}
		m_classColor[QString::fromStdString(className)] = QColor(colors[2], colors[1], colors[0]);
	}

	std::vector<AnnoVis::EmbryoBox> imageBoxes;
	const nlohmann::json& detections = m_images2JSONs[imageIdx].second["detection_list"];

	for (const auto& detection : detections)
	{
		float severe = .0f;
		bool rotated = false;

		if (detection.count("severe")) {
			severe = detection["severe"].get<float>();
		}
		if (detection.count("rotated")) {
			rotated = detection["rotated"].get<bool>();
		}

		bool isConcentrationConfident = false;

		if (detection.count("concentrationConfident")) {
			isConcentrationConfident = detection["concentrationConfident"].get<bool>();
		}

		const int brx = static_cast<int>(detection["brx"]);
		const int bry = static_cast<int>(detection["bry"]);
		const int tlx = static_cast<int>(detection["tlx"]);
		const int tly = static_cast<int>(detection["tly"]);
		const QRect box = QRect(QPoint(tlx, tly), QPoint(brx, bry));
		const size_t id = static_cast<int>(detection["id"]);

		AnnoVis::EmbryoBox embryoBox;
		std::string embryoclassName = detection["className"].get<std::string>();

		embryoBox.className = QString::fromStdString(embryoclassName);
		embryoBox.id = id;
		embryoBox.isHighlighted = false;
		embryoBox.bbox = box;
		embryoBox.color = m_classColor[embryoBox.className];
		embryoBox.isClassConfident = static_cast<bool>(detection["confidence"]);
		embryoBox.isConcentrationConfident = isConcentrationConfident;
		embryoBox.severe = severe;
		embryoBox.isRotated = rotated;
		imageBoxes.push_back(embryoBox);

	}
	m_imagesEmbryoBoxes[imageIdx] = imageBoxes;
}

void AnnoVis::redraw(const size_t& imageIndx)
{
	
	if ((m_images2JSONs.size() == 0) || imageIndx > m_images2JSONs.size() - 1)
	{
		return;
	}

	QImage img(QString::fromStdString(m_images2JSONs[imageIndx].first));
	img.convertTo(QImage::Format_RGB888);
	m_currentIndex = imageIndx;

	std::vector<int> counters;
	counters.resize(m_classColor.size());
	const int penWidth = 5;
	for (const auto& embryoBox : m_imagesEmbryoBoxes[imageIndx])
	{
		const auto pen = ((embryoBox.isHighlighted ? 1 : 0) + 1) * penWidth;
		drawRect(&img, embryoBox.bbox, embryoBox.color, pen, embryoBox.isClassConfident);
#if NOT TEST_MODE 
		drawText(&img, embryoBox.bbox, embryoBox.color, pen, embryoBox.id);
#endif	
		auto severePos = embryoBox.bbox;
		severePos.moveTo(severePos.x(), severePos.y() + 20 * penWidth);
#if NOT TEST_MODE 
		drawText(&img, severePos, embryoBox.color, pen, embryoBox.severe);
#endif		
        counters[m_classNameID[embryoBox.className]]++;

		if (embryoBox.isConcentrationConfident)
		{
			QRect textBox = embryoBox.bbox;
			QPoint TextBoxCenter = embryoBox.bbox.center();
			QPoint new_TopLeft = TextBoxCenter;
			new_TopLeft.setX(new_TopLeft.x() - 0.15 * textBox.width());
			new_TopLeft.setY(new_TopLeft.y() + 0.05 * textBox.height());

			QPoint new_BottomRight = TextBoxCenter;
			new_BottomRight.setX(new_BottomRight.x() + 0.35 * textBox.width());
			new_BottomRight.setY(new_BottomRight.y() + 0.3 * textBox.height());

			textBox.setTopLeft(new_TopLeft);
			textBox.setBottomRight(new_BottomRight);
			drawRect(&img, textBox, embryoBox.color, pen, embryoBox.isConcentrationConfident);
		}

		if (embryoBox.isRotated) {

			QRect textBox = embryoBox.bbox;
			QPoint TextBoxRight = embryoBox.bbox.topRight();
			QPoint new_BottomLeft = TextBoxRight;
			QPoint new_TopRight = TextBoxRight;

			new_TopRight.setX(new_TopRight.x() + 0.3 * textBox.width());
			new_TopRight.setY(new_TopRight.y() - 0.3 * textBox.height());

			new_BottomLeft.setX(new_BottomLeft.x() - 0.8 * textBox.width());
			new_BottomLeft.setY(new_BottomLeft.y() + 0.8 * textBox.height());


			textBox.setBottomLeft(new_BottomLeft);
			textBox.setTopRight(new_TopRight);

			drawText(&img, textBox, embryoBox.color, pen, QString::fromStdString("R"));

		}
	}
	emit sendCounters(counters);
	emit sendImage(img);
}

void AnnoVis::redrawLast()
{
	this->redraw(m_images2JSONs.size() - 1);
}

void AnnoVis::calculateColors(const size_t& imageIndx)
{
	nlohmann::json resultJSON = m_images2JSONs[imageIndx].second;
	for (auto& detectionJSON : resultJSON["detection_list"])
	{
		const int classID = static_cast<int>(detectionJSON["classId"]);

		detectionJSON["className"] = m_classIDName[classID].toStdString();
	}
	for (auto& embryoBox : m_imagesEmbryoBoxes[imageIndx])
	{

		embryoBox.color = m_classColor[embryoBox.className];

	}
}

void AnnoVis::calculateColors()
{
	this->calculateColors(m_images2JSONs.size() - 1);
}

void AnnoVis::setImages2JSONs(const std::vector<std::pair<std::string, nlohmann::json> >& images2JSONs)
{
	m_images2JSONs = images2JSONs;

	m_imagesEmbryoBoxes.clear();
	m_imagesEmbryoBoxes.resize(m_images2JSONs.size());
	for (size_t imageIdx = 0; imageIdx < m_images2JSONs.size(); ++imageIdx)
	{
		this->readImageEmbryoBoxes(imageIdx, m_images2JSONs[imageIdx].second);
	}
}

void AnnoVis::setHighlighted(const QPoint& p)
{
	double distance = DBL_MAX;

	int closestIdx = findClosestEmbryoIndex(p);

	if (closestIdx >= 0)
	{
		m_imagesEmbryoBoxes[m_currentIndex][closestIdx].isHighlighted ^= true;
	}
	redraw(m_currentIndex);
}

void AnnoVis::selectAll()
{  
	if (m_imagesEmbryoBoxes.empty())
	{
		return;
	}
	for (size_t embryo_index = 0; embryo_index < m_imagesEmbryoBoxes[m_currentIndex].size(); embryo_index++)
	{
		m_imagesEmbryoBoxes[m_currentIndex][embryo_index].isHighlighted = true;
	}
	redraw(m_currentIndex);
}

void AnnoVis::unselectAll()
{
	if (m_imagesEmbryoBoxes.empty())
	{
		return;
	}
	for (size_t embryo_index = 0; embryo_index < m_imagesEmbryoBoxes[m_currentIndex].size(); embryo_index++)
	{
		m_imagesEmbryoBoxes[m_currentIndex][embryo_index].isHighlighted = false;
	}
	redraw(m_currentIndex);
}


void AnnoVis::highlightUnknowns()
{
	if (m_imagesEmbryoBoxes.empty())
	{
		return;
	}
	for (size_t embryo_index = 0; embryo_index < m_imagesEmbryoBoxes[m_currentIndex].size(); embryo_index++)
	{
		if (m_imagesEmbryoBoxes[m_currentIndex][embryo_index].className == "UNKNOWN")
		{
			m_imagesEmbryoBoxes[m_currentIndex][embryo_index].isHighlighted ^= true;
		}

	}
	redraw(m_currentIndex);
}




void AnnoVis::getWheelRotation(const QPoint& pos, const QPoint& angle)
{

	int closestIdx = findClosestEmbryoIndex(pos);

	int idToRelabel = -1;

	if (closestIdx >= 0)
	{
		idToRelabel = m_imagesEmbryoBoxes[m_currentIndex][closestIdx].id;
		float severe = m_imagesEmbryoBoxes[m_currentIndex][closestIdx].severe + angle.y();
		severe = std::max(std::min(severe, 100.0f), 0.0f);
		for (size_t f = 0; f < m_images2JSONs.size(); f++)
		{
			for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
			{

				if (idToRelabel == embryoBbox.id)
				{
					embryoBbox.severe = severe;
				}

			}
		}
	}

	redraw(m_currentIndex);
}

void AnnoVis::setConcentration(const int& concentration)
{
	if (m_images2JSONs.empty()) {
		return;
	}

	std::vector <size_t> idsToRelabel;

	for (const auto& embryoBbox : m_imagesEmbryoBoxes[m_currentIndex])
	{
		if (embryoBbox.isHighlighted)
		{
			idsToRelabel.push_back(embryoBbox.id);
		}
	}

	std::set<int> impossibleIDs;

	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			for (const auto& id : idsToRelabel)
			{
				if (id == embryoBbox.id)
				{
					embryoBbox.severe = concentration;
					if (f < m_currentIndex)
					{
						embryoBbox.isConcentrationConfident = false;
					}
					else
					{
						embryoBbox.isConcentrationConfident = true;
					}
				}
			}
		}
	}


	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			embryoBbox.isHighlighted = false;
		}
	}

	this->redraw(m_currentIndex);
}

void AnnoVis::setNewClass(const int& index)
{

	if (m_images2JSONs.empty()) {
		return;
	}

	if (index > m_classNameID.size())
	{
		return;
	}

	std::vector <size_t> idsToRelabel;

	for (const auto& embryoBbox : m_imagesEmbryoBoxes[m_currentIndex])
	{
		if (embryoBbox.isHighlighted)
		{
			idsToRelabel.push_back(embryoBbox.id);
		}

	}
	std::set<int> impossibleIDs;

	for (size_t f = m_currentIndex; f < m_images2JSONs.size(); f++)
	{
		for (const auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			for (const auto& id : idsToRelabel)
			{
				if (id == embryoBbox.id)
				{
#if ENABLE_SWITCH_LOGIC

					if (!isPossible(embryoBbox.className, m_classIDName[index]))
					{
						impossibleIDs.insert(id);
					}

#endif // !ENABLE_SWITCH_LOGIC

				}
			}
		}
	}

	if (!impossibleIDs.empty())
	{
		QMessageBox warning;
		QString idsString;
		for (const auto& id : impossibleIDs)
		{
			idsString += QString::number(id) + "  ";
		}
		warning.setText("wrong class transition for embryo tracks " + idsString);
		warning.exec();
		return;
	}
#if ENABLE_SWITCH_LOGIC
	for (size_t f = m_currentIndex; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			for (const auto& id : idsToRelabel)
			{
				if (id == embryoBbox.id)
				{
					embryoBbox.className = m_classIDName[index];
					embryoBbox.color = m_classColor[m_classIDName[index]];
				}
			}
		}
	}

#elif !ENABLE_SWITCH_LOGIC
	size_t f = m_currentIndex;
	for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
	{
		for (const auto& id : idsToRelabel)
		{
			if (id == embryoBbox.id)
			{
				embryoBbox.className = m_classIDName[index];
				embryoBbox.color = m_classColor[m_classIDName[index]];
			}
		}
	}


#endif // !ENABLE_SWITCH_LOGIC



	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			embryoBbox.isHighlighted = false;
		}
	}
	this->redraw(m_currentIndex);
}

bool AnnoVis::isPossible(const QString oldClass, const QString newClass)
{

	if (std::find(m_possibleTransitions[oldClass].begin(),
		m_possibleTransitions[oldClass].end(),
		newClass) !=
		m_possibleTransitions[oldClass].end())
	{
		return true;
	}
	return false;
}

void AnnoVis::createPossibleTransitions()
{
	m_possibleTransitions["UNKNOWN"] = { "BMP",  "NODAL", "NORMAL", "UNKNOWN", "CUT", "BOOM","WNT", "FGF", "SHH", "PCP", "RA" };
	m_possibleTransitions["BMP"] = { "BMP", "CUT", "BOOM" };
	m_possibleTransitions["NODAL"] = { "NODAL", "CUT", "BOOM" };
	m_possibleTransitions["NORMAL"] = { "NORMAL",  "CUT", "BOOM" };
	m_possibleTransitions["CUT"] = { "BMP",  "NODAL", "NORMAL", "UNKNOWN", "CUT", "BOOM" };
	m_possibleTransitions["BOOM"] = { "BOOM", "CUT" };


	m_possibleTransitions["WNT"] = { "WNT", "BOOM", "CUT" };
	m_possibleTransitions["FGF"] = { "FGF", "BOOM", "CUT" };
	m_possibleTransitions["SHH"] = { "SHH", "BOOM", "CUT" };
	m_possibleTransitions["PCP"] = { "PCP", "BOOM", "CUT" };
	m_possibleTransitions["RA"] = { "RA" , "BOOM", "CUT" };
}

void AnnoVis::clearTracks()
{
	std::vector <size_t> idsToRelabel;

	for (const auto& embryoBbox : m_imagesEmbryoBoxes[m_currentIndex])
	{
		if (embryoBbox.isHighlighted)
		{
			idsToRelabel.push_back(embryoBbox.id);
		}

	}
#if ENABLE_SWITCH_LOGIC

	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			for (const auto& id : idsToRelabel)
			{
				if (id == embryoBbox.id)
				{
					embryoBbox.className = "UNKNOWN";
					embryoBbox.color = m_classColor[embryoBbox.className];
					embryoBbox.isHighlighted = false;
					embryoBbox.isClassConfident = true;
					embryoBbox.isConcentrationConfident = false;
					embryoBbox.severe = false;
				}
			}
		}
	}

#elif !ENABLE_SWITCH_LOGIC
	size_t f = m_currentIndex;
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			for (const auto& id : idsToRelabel)
			{
				if (id == embryoBbox.id)
				{
					embryoBbox.className = "UNKNOWN";
					embryoBbox.color = m_classColor[embryoBbox.className];
					embryoBbox.isHighlighted = false;
					embryoBbox.isClassConfident = true;
					embryoBbox.isConcentrationConfident = false;
					embryoBbox.severe = false;
				}
			}
		}
	}


#endif // !ENABLE_SWITCH_LOGIC

	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			embryoBbox.isHighlighted = false;
		}
	}




	this->redraw(m_currentIndex);
}

void AnnoVis::processSaveRequest()
{
	std::vector <std::pair<std::string, nlohmann::json>>  newImage2Json = formNewJson();
	emit save(newImage2Json);
	m_currentIndex = 0;
	redraw(m_currentIndex);
}

std::vector<std::pair<std::string, nlohmann::json>> AnnoVis::formNewJson()
{
	std::vector <std::pair<std::string, nlohmann::json>> newJsonVector;

	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		std::vector<int> counters;
		counters.resize(m_classColor.size());

		nlohmann::json detectionListJSON;
		std::vector<nlohmann::json> imageJson;
		int i = 0;
		for (const auto& embyobox : m_imagesEmbryoBoxes[f])
		{
			nlohmann::json detectionJson;
			detectionJson["id"] = embyobox.id;
			std::vector<int> colors;
			colors.push_back(embyobox.color.red());
			colors.push_back(embyobox.color.green());
			colors.push_back(embyobox.color.blue());
			detectionJson["color"] = colors;
			detectionJson["className"] = embyobox.className.toStdString();
			detectionJson["classId"] = m_classNameID[embyobox.className];
			detectionJson["brx"] = embyobox.bbox.bottomRight().x();
			detectionJson["bry"] = embyobox.bbox.bottomRight().y();
			detectionJson["tlx"] = embyobox.bbox.topLeft().x();
			detectionJson["tly"] = embyobox.bbox.topLeft().y();
			detectionJson["confidence"] = embyobox.isClassConfident;
			detectionJson["severe"] = embyobox.severe;
			detectionJson["concentrationConfident"] = embyobox.isConcentrationConfident;
			detectionJson["rotated"] = embyobox.isRotated;

			std::vector<float> probs;
			probs.resize(m_classColor.size());
			probs[m_classNameID[embyobox.className]] = 1;
			counters[m_classNameID[embyobox.className]] += 1;
			detectionJson["prob"] = probs;
			imageJson.push_back(detectionJson);
		}
		detectionListJSON["detection_list"] = imageJson;
		detectionListJSON["source_name"] = m_images2JSONs[f].second["source_name"];
		detectionListJSON["class_count"] = counters;
		detectionListJSON["version"] = m_images2JSONs[f].second["labeler"];
		newJsonVector.push_back({ m_images2JSONs[f].first ,detectionListJSON });
	}
	return newJsonVector;

}

void AnnoVis::setConfident(const bool& confident)
{
	if (m_imagesEmbryoBoxes.size() == 0)
	{
		return;
	}

	std::vector<size_t> idsToRelabel;

	for (const auto& embryoBbox : m_imagesEmbryoBoxes[m_currentIndex])
	{
		if (embryoBbox.isHighlighted)
		{
			idsToRelabel.push_back(embryoBbox.id);
		}
	}

	for (size_t f = m_currentIndex; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			for (const auto& id : idsToRelabel)
			{
				if (id == embryoBbox.id)
				{
					embryoBbox.isClassConfident = confident;
				}
			}
		}
	}

	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			embryoBbox.isHighlighted = false;
		}
	}
	this->redraw(m_currentIndex);
}


int AnnoVis::findClosestEmbryoIndex(const QPoint& pos)
{

	if (m_imagesEmbryoBoxes.size() == 0)
	{
		return -1;
	}

	double distance = DBL_MAX;
	int closestIdx = -1;

	for (size_t i = 0; i < m_imagesEmbryoBoxes[m_currentIndex].size(); ++i)
	{
		bool isInside = false;
		const auto& imageEmbryoBoxes = m_imagesEmbryoBoxes[m_currentIndex][i];

		const double current_distance = std::pow(std::pow(imageEmbryoBoxes.bbox.center().x() - pos.x(), 2) +
			std::pow(imageEmbryoBoxes.bbox.center().y() - pos.y(), 2), 0.5);

		isInside = imageEmbryoBoxes.bbox.contains(pos);

		if (isInside && current_distance < distance)
		{
			distance = current_distance;
			closestIdx = static_cast<int>(i);
		}
	}
	return closestIdx;
}


void AnnoVis::wheelPress(const QPoint& pos)
{
	int closestIdx = findClosestEmbryoIndex(pos);
	if (closestIdx < 0)
	{
		return;
	}

	int idToRelabel = -1;
	idToRelabel = m_imagesEmbryoBoxes[m_currentIndex][closestIdx].id;

	const bool isCurrentConcentrationConfident = m_imagesEmbryoBoxes[m_currentIndex][closestIdx].isConcentrationConfident;

	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{

			if (idToRelabel == embryoBbox.id)
			{
				if (f < m_currentIndex)
				{
					embryoBbox.isConcentrationConfident = false;
				}
				else
				{
					embryoBbox.isConcentrationConfident = !isCurrentConcentrationConfident;
				}

			}

		}
	}
	this->redraw(m_currentIndex);
}

void AnnoVis::changeSingleEmbryoConfidence(const QPoint& pos)
{
	int closestIdx = findClosestEmbryoIndex(pos);
	if (closestIdx < 0)
	{
		return;
	}

	int idToRelabel = -1;
	idToRelabel = m_imagesEmbryoBoxes[m_currentIndex][closestIdx].id;

	const bool isClassConfident = m_imagesEmbryoBoxes[m_currentIndex][closestIdx].isClassConfident;

	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{

			if (idToRelabel == embryoBbox.id)
			{
				if (f < m_currentIndex)
				{
					embryoBbox.isClassConfident = false;
				}
				else
				{
					embryoBbox.isClassConfident = !isClassConfident;

				}
			}
		}

	}
	this->redraw(m_currentIndex);
}

void AnnoVis::getRotation()
{
	//*********************************************************************

	if (m_images2JSONs.empty()) {
		return;
	}

	std::vector <size_t> idsToRelabel;

	for (const auto& embryoBbox : m_imagesEmbryoBoxes[m_currentIndex])
	{
		if (embryoBbox.isHighlighted)
		{
			idsToRelabel.push_back(embryoBbox.id);
		}
	}

	for (size_t f = m_currentIndex; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			for (const auto& id : idsToRelabel)
			{
				if (id == embryoBbox.id)
				{
					embryoBbox.isRotated = !embryoBbox.isRotated;
				}
			}
		}
	}


	for (size_t f = 0; f < m_images2JSONs.size(); f++)
	{
		for (auto& embryoBbox : m_imagesEmbryoBoxes[f])
		{
			embryoBbox.isHighlighted = false;
		}
	}
	this->redraw(m_currentIndex);

	//*********************************************************************************


}