#pragma once


#if defined(linux) || defined(__linux)

#include "ScreenCapture.hpp"

#include <opencv2/opencv.hpp>
#include <X11/Xutil.h>

namespace CVUtils
{
class LinuxScreenCapture : public ScreenCapture
{
	int screenWidth, screenHeight;
	XImage *ximage;
	XWindowAttributes gwa;
	Display *screenDisplay;
	Window root;

	void prepareScreenParametres();
	void clean();
public:
	void captureScreen(cv::Mat &image);
	LinuxScreenCapture();
};
}

#endif
