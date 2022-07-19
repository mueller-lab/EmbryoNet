#pragma once

/** @file */

#include "ImageProcessor.hpp"

namespace CVUtils
{

	namespace IP
	{

		class Cropper : virtual public ImageProcessor
		{
		public:
			/** @brief Transform constructor.

			The mat parameter is internally transformed to CVUtils::Image, which is underlying image storage type in this class.

			@param mat cv::Mat representation of the image to be processed.
			*/
			Cropper(cv::Mat& mat);

			/** @brief Transform constructor.

			@param img CVUtils::Image representation of the image to be processed.
			*/
			Cropper(const CVUtils::Image &img);

			Cropper &roi(size_t x1, size_t y1, size_t x2, size_t y2);

		};

	}

}
