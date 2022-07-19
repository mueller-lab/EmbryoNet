#pragma once

#include "DSDetector.h"
#include "DSEncoder.h"

#include "Matcher.h"
#include "KalmanFilter.h"
#include "TrackUpdater.h"

#include "DSConfig.h"

#include <memory>

//
// The DeepSort tracking algorithm.
// Interface of the tracker.
// 
// The class is constructed by Detector, Encoder subclasses
// and optional DSConfig parameter.
//
// If the user does not have an Encoder algorithm, nullptr should be passed.
// In that case the matching is performed only via position correspondence,
// based on Kalman Filters' predctions and InterSetction over Union metric.
// Essentially, it is a way to get SORT MOT algorithm implementation.
//
// Refer to the header files of those for detailed info.
//
// ProcessFrame is the main method of the algorithm. 
// Given input frame 
//
// 1) Calls Detector::detect to obtain vector of object-class bounding boxes
// 2) If Encoder is given: calls Encoder::encode to obtain vector of bounding box features
// 3) Matches bounding boxes to existing tracks via Matcher algorithm
// 4) Updates list of tracks via TrackUpdater algorithm.
//
// After construction the user should call processFrame every consequent frame
// for the algorithm to track with the best quality.
// However it is not a must-requirement.
//

class DeepSort
{
	std::shared_ptr<DSDetector> m_detector;
	std::shared_ptr<DSEncoder> m_encoder;

	std::unique_ptr<Matcher> m_matcher;
	std::unique_ptr<TrackUpdater> m_trackUpdater;

	std::shared_ptr<MatchResult> m_matchResult;

	std::vector<std::vector<float> > m_features;
	std::vector<CVUtils::BBoxf> m_bboxes;

public:

	DeepSort(std::shared_ptr<DSDetector> detector, std::shared_ptr<DSEncoder> encoder, const DSConfig &config = DSConfig());
	~DeepSort();

	//
	// The main method the user calls for the tracker
	// to process current frame and update tracking information.
	//
	void processFrame(const cv::Mat &frame);

	// 
	// Bouding boxes given by Detector
	//
	std::vector<CVUtils::BBoxf> bboxes() const;
	//
	// All tracks: including matched confirmed, tentaive and unmatched confirmed.
	//
	std::vector<Track> allTracks() const;

	//
	// Only confirmed tracks which were successfully matched in current frame. 
	// This method is the one user is most likely to use, that's why
	// it is called tracks(), though the name is incorrect.
	//
	std::vector<Track> tracks() const;

};