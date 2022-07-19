#pragma once

#include "Track.h"

#include "Bbox.hpp"

struct DSDetection
{
private:

	CVUtils::Bboxf m_bbox;
	std::vector<float> m_features;

	size_t m_id;

public:

	DSDetection(const CVUtils::BBoxf &bbox, const std::vector<float> &features, size_t id);
	DSDetection(const DSDetection &other);

	float iou(const CVUtils::Bboxf &bbox) const;
	
	CVUtils::Bboxf bbox() const;
	std::vector<float> features() const;

	// computed via id for std::set in Matcher.cpp
	bool operator < (const DSDetection &other) const;
};

struct Match
{
private:

	Track m_track;
	DSDetection m_detection;
	
public:

	Match(const Track &track, const DSDetection &detection);
	Match(const Match &other);

	Track track() const;
	DSDetection detection() const;
	
	CVUtils::Bboxf bbox() const;
	std::vector<float> features() const;

};

struct MatchResult
{
private:

	std::vector<Match> m_match;
	std::vector<DSDetection> m_unmatchedDetections;

public:

	MatchResult(const std::vector<Match> &match,
		        const std::vector<DSDetection> &unmatchedDetections);

	std::vector<Match> match() const;
	std::vector<DSDetection> unmatchedDetections() const;

};
