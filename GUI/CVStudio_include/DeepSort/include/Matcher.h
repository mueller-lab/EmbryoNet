#pragma once

#include "Track.h"
#include "DSTypes.h"

#include "DSConfig.h"

//
// Matching algorithm implementation.
// 
// Matching is performed in two steps.
// 1) Matching cascade by features.
// This step is ignored, if no Encoder is given to DeepSort algorithm.
// 2) Intersection over Union SORT-style matching
// of the remaining after first step tracks and detections.
//

class Matcher
{
private:

	size_t m_ageMax;
	float m_lambda;
	float m_cosMax, m_iouMax, m_mahaMax;

public:

	std::shared_ptr<MatchResult> match(const std::vector<DSDetection> &detections,
                                       const std::vector<Track> &tracks) const;

	Matcher(const DSConfig &config);
	~Matcher();

private:

	std::shared_ptr<MatchResult> matchByPos(const std::vector<DSDetection> &detections,
                                            const std::vector<Track> &tracks) const;

	std::shared_ptr<MatchResult> matchByFeatures(const std::vector<DSDetection> &detections,
                                                 const std::vector<Track> &tracks) const;


private:

	float featureDistance(const Track &track, const DSDetection &detection) const;
	float cosineDistance(const std::vector<float> &a, const std::vector<float> &b) const;

	float maha2(const CVUtils::Bboxf &a, const CVUtils::Bboxf &b, const cv::Mat &S) const;
};