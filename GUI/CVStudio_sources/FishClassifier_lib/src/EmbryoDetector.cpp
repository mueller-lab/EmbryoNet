#include "EmbryoDetector.h"

class HoughInitData
{

public:

	HoughInitData(const nlohmann::json& config);

	inline auto method() const
	{
		return m_metod;
	}

	inline auto dp() const
	{
		return m_dp;
	}

	inline auto minDist() const
	{
		return m_minDist;
	}

	inline auto param1() const
	{
		return m_param1;
	}
	
	inline auto param2() const
	{
		return m_param2;
	}
	
	inline auto minRadius() const
	{
		return m_minRadius;

	}
	
	inline auto maxRadius() const
	{
		return m_maxRadius;
	}

private:

	int m_metod;
	int m_dp;
	int m_minDist; 
	int m_param1; 
	int m_param2; 
	int m_minRadius; 
	int m_maxRadius; 
};


EmbryoDetector::~EmbryoDetector()
{
}

EmbryoDetector::EmbryoDetector(const nlohmann::json& config):
	m_fx(static_cast<float>(config["fx"])),
	m_fy(static_cast<float>(config["fy"])),
	m_houghInitData(std::make_unique<HoughInitData>(config["hough"]))
{
}

void EmbryoDetector::detect(const cv::Mat& frame, std::vector<CVUtils::BBoxf>& bboxes)
{
	bboxes.clear();
	cv::Mat matCopy = frame.clone();

	cv::cvtColor(matCopy, matCopy, cv::COLOR_RGB2GRAY);
	//cv::equalizeHist(matCopy, matCopy);
	cv::medianBlur(matCopy, matCopy, 5);
	

	cv::resize(matCopy, matCopy, cv::Size(), m_fx, m_fy);

	std::vector<cv::Vec3f> outputCicles;  
	cv::HoughCircles(
		matCopy,
		outputCicles,
		m_houghInitData->method(),
		m_houghInitData->dp(),
		m_houghInitData->minDist(),
		m_houghInitData->param1(),
		m_houghInitData->param2(),
		m_houghInitData->minRadius(),
		m_houghInitData->maxRadius()
	);

	for (size_t i = 0; i < outputCicles.size(); i++)
	{
		cv::Point center(cvRound(outputCicles[i][0]), cvRound(outputCicles[i][1]));
		const int radius = cvRound(outputCicles[i][2]);
		const float tlx = std::max(0, center.x - radius) / m_fx;
		const float tly = std::max(0, center.y - radius) / m_fy;
		const float brx = std::min(center.x + radius, matCopy.cols) / m_fx;
		const float bry = std::min(center.y + radius, matCopy.rows) / m_fy;
		const cv::Rect_<float> bbox(cv::Point_<float>(tlx, tly), cv::Point_<float>(brx, bry));
		bboxes.push_back(bbox);
	}
}

HoughInitData::HoughInitData(const nlohmann::json& config)
{
	const std::map<std::string, int> houghMap =
	{
		{ "HOUGH_GRADIENT",     cv::HOUGH_GRADIENT },
		{ "HOUGH_GRADIENT_ALT", cv::HOUGH_GRADIENT_ALT }
	}; 

	const std::string methodStr = config["method"];
	m_metod     = houghMap.at(methodStr);

	m_dp        = static_cast<int>(config["dp"]); 
	m_minDist   = static_cast<int>(config["minDist"]); 
	m_param1    = static_cast<int>(config["param1"]); 
	m_param2    = static_cast<int>(config["param2"]); 
	m_minRadius = static_cast<int>(config["minRadius"]);
	m_maxRadius = static_cast<int>(config["maxRadius"]);
}								   

