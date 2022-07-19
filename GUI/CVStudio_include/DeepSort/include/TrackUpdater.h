#pragma once

#include <vector>

#include "Track.h"
#include "DSTypes.h"
#include "DSConfig.h"

//
// TrackUpdater is the class that controls
// 1) creation of tracks by unmatched detections;
// 2) confirmation of tracks;
// 3) update of confirmed tracks;
// 4) miss marking of unmatched tracks.
// 
class TrackUpdater
{
	size_t m_ageMax;

	std::vector<Track> m_tracks;
	size_t m_newTrackId;
	size_t m_newConfirmedTrackId;

	DSConfig m_config;

public:

	TrackUpdater(const DSConfig &config);
	~TrackUpdater();

	void predict();
	void update(const std::shared_ptr<MatchResult> matchResult);

	std::vector<Track> tracks() const;
};