#pragma once

#include <memory>

#include "KalmanFilter.h"
#include "DSConfig.h"

//
// Track state: Tentaive or Confirmed. 
// There is no separate state for Deleted tracks in this implementation: 
// deleted tracks are being deleted immediately.
//
enum class TrackState
{
	TENTATIVE = 0, CONFIRMED = 1
};

//
// Track class.
// From end-user's perspective 
// each Track has state, id and position.
//

class Track
{
	size_t m_id, m_cofirmedId;
	size_t m_age;
	size_t m_hits;

	size_t m_nMax;

	TrackState m_state;

	std::shared_ptr<KalmanFilter> m_kf;
	std::vector<std::vector<float> > m_featureGallery;

public:

	Track(size_t id, const CVUtils::Bboxf &bbox, const std::vector<float> &features, const DSConfig &config);
	Track(const Track &other);

	~Track();

	void predict();
	void update(const CVUtils::Bboxf &bbox, const std::vector<float> &features);

	void processMiss();

	void confirm(size_t confirmId);

	//
	// m_id: actual track id, including tentative track history.
	// This may be misleading for developers, but is better
	// for end-users, who don't know anything about tentative tracks
	size_t actualId() const;

	//
	// Returns confirmed id. To get m_id call actuailId().
	// This may be misleading for developers, but is better
	// for end-users, who don't know anything about tentative tracks
	size_t id() const;
	size_t age() const;

	size_t hits() const;

	TrackState state() const;

	CVUtils::Bboxf pos() const;
	cv::Mat S() const;

	std::vector<std::vector<float> > featureGallery() const;

	bool operator < (const Track &other) const;
};