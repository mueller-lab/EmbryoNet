#pragma once

/** @file */

#include "ImageProcessor.hpp"

namespace CVUtils
{

namespace IP 
{
/** @brief Provides the operations which change width, height, depth and internal data type of the image.

ShapeType is a class, designed to provide the most common image processing operations. 
Those include: width, height, depth and internal data type conversion.

*/

class ShapeType: virtual public ImageProcessor
{
public:
	/** @brief Transform constructor.

	The mat parameter is internally transformed to CVUtils::Image, which is underlying image storage type in this class.

	@param mat cv::Mat representation of the image to be processed.
	*/
    ShapeType(cv::Mat& mat);

	/** @brief Transform constructor.

	@param img CVUtils::Image representation of the image to be processed.
	*/
    ShapeType(const CVUtils::Image &img);
    
    
/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor. 
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT. 
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param constructInfo ImageConstructInfo of the converted image.
	*/
	ShapeType& convert(const ImageConstructInfo &constructInfo);

	/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param shape3D Shape3D of the converted image.
	@param type specifies the intern(al data type of the converted image.
	*/
	ShapeType& convert(const Shape3D &shape3D, const ImageDataType &type);

	/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param size Size of the converted image.
	@param depth Shape3D::Depth of the converted image.
	@param type specifies the internal data type of the converted image.
	*/
	ShapeType& convert(const Size &size, const Shape3D::Depth &depth, const ImageDataType &type);

	/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param size Size of the converted image.
	@param channels specifies the number of channels of the converted image.
	@param type specifies the internal data type of the converted image.
	*/
	ShapeType& convert(const Size &size, size_t channels, const ImageDataType &type);

	/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param width width of the converted image.
	@param height height of the converted image.
	@param depth Shape3D::Depth of the converted image.
	@param type specifies the internal data type of the converted image.
	*/
	ShapeType& convert(size_t width, size_t height, const Shape3D::Depth &depth, const ImageDataType &type);

	/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param width width of the converted image.
	@param height height of the converted image.
	@param channels specifies the number of channels of the converted image.
	@param type specifies the internal data type of the converted image.
	*/
	ShapeType& convert(size_t width, size_t height, size_t channels, const ImageDataType &type);

	/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param size Size of the converted image.
	@param cvType OpenCV - style int value, encoding internal data type and number of channels.
	*/
	ShapeType& convert(const Size &size, int cvType);

	/** @brief Change width, height, depth and the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param width width of the converted image.
	@param height height of the converted image.
	@param cvType OpenCV - style int value, encoding internal data type and number of channels.
	*/
	ShapeType& convert(size_t width, size_t height, int cvType);

	/** @brief Change the internal data type of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param type specifies the internal data type of the converted image.
	*/
	ShapeType& convertType(const ImageDataType &dataType);

	/** @brief Change width, height and depth of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param shape3D Shape3D of the converted image.
	*/
	ShapeType& reshape3D(const Shape3D &shape);

	/** @brief Change width, height and depth of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param size Size of the converted image.
	@param depth Shape3D::Depth of the converted image.
	*/
	ShapeType& reshape3D(const Size &size, const Shape3D::Depth &depth);

	/** @brief Change width, height and depth of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param size Size of the converted image.
	@param channels specifies the number of channels of the converted image.
	*/
	ShapeType& reshape3D(const Size &size, size_t channels);

	/** @brief Change width, height and depth of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param width width of the converted image.
	@param height height of the converted image.
	@param depth Shape3D::Depth of the converted image.
	*/
	ShapeType& reshape3D(size_t width, size_t height, const Shape3D::Depth &depth);

	/** @brief Change width, height and depth of the image.

	Please note, that only positive width, height and depth are allowed. Other values will result in std::runtime_error.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.

	@param width width of the converted image.
	@param height height of the converted image.
	@param channels specifies the number of channels of the converted image.
	*/
	ShapeType& reshape3D(size_t width, size_t height, size_t channels);

	/** @brief Change width and height of the image.

	Please note, that only positive width and height are allowed. Other values will result in std::runtime_error.

	@param size Size of the converted image.
	*/
	ShapeType& resize(const Size &size);

	/** @brief Change width and height of the image.

	Please note, that only positive width and height are allowed. Other values will result in std::runtime_error.

	@param width width of the converted image.
	@param height height of the converted image.
	*/
	ShapeType& resize(size_t width, size_t height);

	/** @brief Converts image to grayscale.

	Size and internal data type remain the same.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.
	*/
	ShapeType& toGrayscale();

	/** @brief Converts image to 3 channel.

	Size and internal data type remain the same.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.
	*/
	ShapeType& to3Channel();

	/** @brief Converts image to 4 channel.

	Size and internal data type remain the same.

	Please note, that conversions, which change depth of the image internally call cv::cvtColor.
	It, in turn, means, that internal data type of the image to be transformed must be one of 1) UNSIGNED_CHAR, 2) UNSIGNED_SHORT, 3) FLOAT.
	Any other case will result in a throw of cv::Exception. For more details, please refer to OpenCV's cv::cvtColor documentation.
	*/
	ShapeType& to4Channel();
};
    
}

}
