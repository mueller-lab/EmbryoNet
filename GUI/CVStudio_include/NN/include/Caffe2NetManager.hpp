#pragma once

#ifdef WITH_CAFFE2

#include "NetManager.hpp"

#include <caffe2/core/init.h>
#include <caffe2/core/net.h>
#include <caffe2/core/context_gpu.h>
#include <caffe2/utils/proto_utils.h>


#ifndef CAFFE2_BASE_DEVICE
//
// We suppose, that Caffe2NetManager is used for GPU.
// If you want to compute on CPU, prefer OpenVINONetManager instead.
//
#define CAFFE2_BASE_DEVICE c10::DeviceType::CUDA
#endif

#ifndef CAFFE2_BASE_INPUT_NAME
//
// Input image is stored in blob, called "data".
// Yes, this is hardcode, but we need to choose some reasonable name for the API
// and stay with it.
//
#define CAFFE2_BASE_INPUT_NAME c10::string("data")
#endif

class Caffe2NetManager final: public NetManager
{
public:

	Caffe2NetManager(const std::string& architecture, const std::string& weights, const std::string& outputBlobName);
	~Caffe2NetManager();
	
	void forward(const CVUtils::Image &input) override;

	float* outputData() override;
	const float* constOutputData() override;

	std::vector<int> inputShape()  const override;
	std::vector<int> outputShape() const override;

	void resizeInput(const CVUtils::Size &newSize) override;
	
	void reshape(const CVUtils::Shape3D &newShape);

	caffe2::Tensor* createBlob(const c10::string& blobName);
	const caffe2::Tensor& getBlob(const c10::string& blobName) const;

	c10::DeviceType getDeviceType() const;

private:

	std::unique_ptr<caffe2::NetBase> m_net;
	std::unique_ptr<caffe2::Workspace> m_workspace;

	const c10::DeviceType m_deviceType;
	
	const c10::string m_inputBlobName;
	std::string m_outputBlobName;

	CVUtils::Shape3D m_inputShape3D;

};


#endif // WITH_CAFFE2

