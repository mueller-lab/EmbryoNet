#pragma once

#include <cstddef>

// Full DeepSort Config in one structure
struct DSConfig
{

private:

	// Number of frames during which tentative track must be matched to become confirmed.
	// If tentative track is missed after less than tInit frames from its creation it will be deleted.
	size_t m_tInit = 3;
	
	// Maximum number of frames that confirmed track can be missed.
	// It is deleted if is being missed more than ageMax consequent frames.
	size_t m_ageMax = 30;

	// Maximim capacity of feature gallery for a single track.
	// If this number is succeeded, than new features are added to the end of the gallery, 
	// and the most old features are deleted.
	size_t m_nMax = 100;

	// Cosine metric gate for a feature match to be considered valid
	float m_cosMax = 0.2f;
	// Intersection over Union gate for a position match to be considered valid
	float m_iouMax = 0.7f;
	// Mahalanobis metric gate for a feature match to be considered valid
	float m_mahaMax = 9.4887f;

	// Lambda parameter from the original DeepSort paper
	// Feature cost matrix is computed as
	// lambda * MAHA_COST + (1 - lambda) * COSINE_COST
	// Must be in the segment [0, 1]. There is an assertion for this in DEBUG mode.
	float m_lambda = .0f;

	// The following two Kalman Filter coefficients are multiplied by height of the bounding box 
	// (given by state) and squared after the multiplication. That's how covariance matrices are generated.

	// Positon coefficient for covariance matrices in Kalman Filter
	float m_cPos = 1.0f / 20.0f;
	// Velocity coefficient for covariance matrices in Kalman Filter
	float m_cVel = 1.0f / 120.0f;

	// Gamma standard deviation
	float m_posCov = 1e-2f;
	// Gamma velocity standard deviation
	float m_velCov = 1e-5f;

public:

	DSConfig();
	DSConfig(const size_t tInit, const size_t ageMax, const size_t nMax, 
             const float cosMax, const float iouMax, const float mahaMax,
             const float lambda, const float cPos, const float cVel,
             const float posCov, const float posVel);

	DSConfig(const DSConfig &other);

	~DSConfig();

	void setTInit(const size_t tInit);
	void setAgeMax(const size_t ageMax);
	void setNMax(const size_t nMax);

	void setCosMax(const float cosMax);
	void setIouMax(const float iouMax);
	void setMahaMax(const float mahaMax);

	void setLambda(const float lambda);

	void setCPos(const float cPos);
	void setCVel(const float cVel);

	void setPosCov(const float posCov);
	void setVelCov(const float posVel);

	size_t tInit() const;
	size_t ageMax() const;
	size_t nMax() const;
	
	float cosMax() const;
	float iouMax() const;
	float mahaMax() const;

	float lambda() const;

	float cPos() const;
	float cVel() const;

	float posCov() const;
	float velCov() const;
};
