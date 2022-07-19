#pragma once

#include "Streamer.hpp"

/** @file */

namespace CVUtils
{
	class WebCamStreamer : public Streamer
	{
		cv::VideoCapture m_cap;
		CVUtils::Image m_img;

	public:

		WebCamStreamer(size_t webCamId);
		~WebCamStreamer();

		Streamer& operator >> (Image &img);

		bool isOpened() const;
		void close();
	};
}