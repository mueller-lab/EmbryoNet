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

#include <memory>

#include <opencv2/imgproc.hpp>
#include "nlohmann/json.hpp"

#include "DeepSort.h"

class HoughInitData;

class EmbryoDetector: public DSDetector
{

public:

	EmbryoDetector(const nlohmann::json& config);
	~EmbryoDetector();

	void detect(const cv::Mat& frame, std::vector<CVUtils::BBoxf>& bboxes) override;

private:

	const float m_fx;
	const float m_fy;

	std::unique_ptr<HoughInitData> m_houghInitData;

};
