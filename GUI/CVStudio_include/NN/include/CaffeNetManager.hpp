#pragma once

#ifdef WITH_CAFFE

#include "NetManager.hpp"

#ifndef USE_OPENCV
#define USE_OPENCV
#endif

#ifdef _MSC_VER
#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif
#endif

#include <caffe/caffe.hpp>

class CaffeNetManager: public NetManager
{
	std::shared_ptr<caffe::Net<float> > m_net;

public:

	CaffeNetManager(const std::string &prototxt, const std::string &caffemodel = std::string());
	~CaffeNetManager();

	void forward(const CVUtils::Image &input) override;

	float *outputData() override;
	const float *constOutputData() override;

	std::vector<int> inputShape() const override;
	std::vector<int> outputShape() const override;

	void resizeInput(const CVUtils::Size &newSize) override;

	const caffe::Blob<float>& getBlobByName(const std::string &name);

	void setDevice(const caffe::Caffe::Brew &device);

private:

	void convertImgToBlob(const CVUtils::Image &img, caffe::Blob<float> &blob);
};

#endif