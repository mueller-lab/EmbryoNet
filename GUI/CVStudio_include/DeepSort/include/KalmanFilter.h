#pragma once

#include "Bbox.hpp"
#include "DSConfig.h"

// DeepSort Kalman Filter implementation
class KalmanFilter
{
	// state is [x, y, gamma, h, x', y', gamma', h'], where gamma is bbox aspect ratio
	// measurement is [x, y, gamma, h]
	const size_t m_stateSize = 8, m_measurementSize = 4;

	// state matrix, measurement matrix
	cv::Mat m_A, m_H;
	// dyanamic system's state and covariance
	cv::Mat m_x, m_P;
	// covariance projection on measurement space
	cv::Mat m_S;
	// measurement and innovation
	cv::Mat m_y, m_v;

	// dynamic system's and measurement's noise covariance matrices
	cv::Mat m_Q, m_R;

	// Kalman gain
	cv::Mat m_K;

	// mulitpliers for position and velocity
	const float m_cPos, m_cVel;
	// diagonal elements for pos and vel covariances
	const float m_posCov, m_velCov;

private:

	void initTransitionAndMeasurementMatrices();

	void initState(const cv::Mat &measurement);
	void initCovariance(const cv::Mat &measurement);

	void updateDynSystemNoiseCovariance();
	void updateMeasurementNoiseCovariance();

public:

	KalmanFilter(const CVUtils::Bboxf &bbox, const DSConfig &config);
	~KalmanFilter() {};

	void predict();
	void update(const CVUtils::Bboxf &measurement);

	CVUtils::Bboxf pos() const;
	cv::Mat S() const;

};