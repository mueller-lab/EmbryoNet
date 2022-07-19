#pragma once

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

#include <memory>

struct Detection
{
    int classId;
    float confidence;
    cv::Rect bbox;
};

class CVNetManager
{
public:

	CVNetManager(const std::string &tensorfile, bool binary);
    CVNetManager(const std::string &prototxt, const std::string &caffemodel);
	CVNetManager(const std::string& onnx, const int inputChannels = 3);
    ~CVNetManager();

    void reshape(int w, int h);

	std::vector<cv::Rect> getBBoxesSSD(const cv::Mat &input,  float confThreashold = 0.5f);
	std::vector<cv::Rect> getClassBBoxesSSD(const cv::Mat &input, float confThreashold, int classId);

	void showDetectionsSSD(const cv::Mat &input, float confThreshold = 0.5f);
	void showDetectionsSSD(cv::VideoCapture cap, float confThreshold = 0.5f);

    std::vector<std::vector<cv::Mat> >  getFeatureMaps (const cv::Mat &input);
    std::vector<Detection> getDetectionsSSD(const cv::Mat &input, float confThreashold);

    std::vector <std::string> getLayersNames();
	cv::Mat forward(cv::Mat &img);

private:

    class CVNetManagerImpl;
    CVNetManagerImpl* m_impl;

public:

	int inChannels();

	bool firstIsConvolutional();

	std::map<std::string, std::string> getLayerTypes();
	std::vector<std::string> getLayerNames();

	std::shared_ptr<cv::dnn::Net> net() const;
	
private:

	std::vector<std::string> m_layerNames;
	std::map<std::string, std::string> m_layerTypes;
};
