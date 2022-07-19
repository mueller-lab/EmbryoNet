#pragma once

#include "Streamer.hpp"

/** @file */

#if defined(_WIN32) || defined (_WIN64) || defined(linux) || defined(__linux)

#include "Stream/Screen/ScreenCapture.hpp"

namespace CVUtils
{
class ScreenStreamer:public Streamer
{
	ScreenCapture *capture;
public:

	ScreenStreamer();
	~ScreenStreamer();

	Streamer& operator >> (Image &img);

	bool isOpened() const;
	void close();
};
}

#endif