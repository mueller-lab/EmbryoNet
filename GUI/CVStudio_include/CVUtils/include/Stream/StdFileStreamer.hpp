#pragma once

#include "Streamer.hpp"

/** @file */

namespace CVUtils
{
	class StdFileStreamer : public Streamer
	{
		cv::VideoCapture m_cap;
		CVUtils::Image m_img;

	public:

		StdFileStreamer(const std::string &fileName);
		~StdFileStreamer();

		Streamer& operator >> (Image &img);

		bool isOpened() const;
		void close();
	};
}
