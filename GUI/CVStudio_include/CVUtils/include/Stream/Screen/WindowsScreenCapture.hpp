#pragma once



#if defined(_WIN32) || defined (_WIN64)

#include "ScreenCapture.hpp"

#include <opencv2/opencv.hpp>
#include <Windows.h>

namespace CVUtils
{
class WindowsScreenCapture : public ScreenCapture
{
	HDC hwindowDC, hwindowCompatibleDC;
	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	BITMAPINFOHEADER  bi;
	RECT windowsize;

	void prepareScreenParametres();
	void clean();
public:
	void captureScreen(cv::Mat &image);
};
}

#endif
