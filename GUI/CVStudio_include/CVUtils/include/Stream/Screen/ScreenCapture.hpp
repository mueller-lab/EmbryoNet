#pragma once

#if defined(_WIN32) || defined (_WIN64) || defined(linux) || defined(__linux)

namespace cv
{
	class Mat;
}

namespace CVUtils
{

class ScreenCapture
{
public:
	virtual void captureScreen(cv::Mat &image) = 0;
	virtual ~ScreenCapture() {}
};

}

#endif