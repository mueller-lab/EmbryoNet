#pragma once

#include "Streamer.hpp"

/** @file */

namespace CVUtils
{
	class PathStreamer: public Streamer
	{
		std::vector<cv::String> paths;

	public:

		PathStreamer(const std::string &path, bool recursive = false);
		~PathStreamer();

		Streamer& operator >>(Image &img);
		cv::String currentPath() const;
		bool isOpened() const;
		void close();

		std::vector<std::string> getPaths() const;
	};
}

