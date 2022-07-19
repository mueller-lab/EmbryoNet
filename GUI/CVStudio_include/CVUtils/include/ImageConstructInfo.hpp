#pragma once

#include "Shape3D.hpp"

/** @file */
namespace CVUtils
{
/** @enum mapper::ImageDataType
*	@brief Image's internal data type 
*
*	It is implemented as a separate data type, unlike OpenCV's style, where image type encodings
*	mix type with number of channels.\n\n
*   E.g. CV_32FC3 means, that image is both float-based
*   and three-channeled, while here these properties are explicitly separated.\n
*
*/
enum class ImageDataType
{
	UNSIGNED_CHAR = 0, /**< Unsigned char. OpenCV code example: CV8U. */
	UCHAR = 0, /**< Alias for UNSIGNED_CHAR. */
	SIGNED_CHAR = 1, /**< Signed char. OpenCV code example: CV8SC4. */
	CHAR = 1, /**< Alias for SIGNED_CHAR. */
	UNSIGNED_SHORT = 2, /**< Unsigned short. OpenCV code example: CV16UC3. */
	USHORT = 2, /**< Alias for UNSIGNED_SHORT. */
	SIGNED_SHORT = 3, /**< Signed short. OpenCV code example: CV16SC1. */
	SHORT = 3, /**< Alias for SIGNED_SHORT. */
	SIGNED_INT = 4, /**< Signed int. OpenCV code example: CV32SC3. */
	INT = 4, /**< Alias for SIGNED_INT. */
	FLOAT = 5, /**< Float. OpenCV code example: CV_32FC4. */
	DOUBLE = 6, /**< Double. OpenCV code example: CV64F. */
	TYPE_NONE = -1 /**< No other internal data type is supported.*/
}; 

/**  @brief Provides full information about image size, number of channels and internal data type.
	
	Class ImageConstructInfo conviniently stores whole information about image construction.
	This information consists of image's number of rows, columns, channels as well as internal image data type.\n
	You may get string representation of ImageConstructInfo via ImageConstructInfo::toString() method call.\n\n
	Please note, that supported image channels are only 0, 1, 3 and 4.
*/

class ImageConstructInfo
{
	Shape3D _shape3D;
	ImageDataType _type;

public:

	/** @brief Default constructor.

		Default constructor sets width, height and number of channels to 0 and internal data type to ImageDataType::TYPE_NONE.\n
		It is not intended to be used in normal situations.
	*/
	ImageConstructInfo();

	/** @brief Transform constructor.
	
		@param shape3D 3D representation of image's width, height and number of channels.
		@param type internal image data type.
	*/
	ImageConstructInfo(const Shape3D &shape3D, const ImageDataType &type);
	/** @brief Transform constructor.

	Possible depth values are FLAT (0 channels), GRAYSCALE (1 channel), THREE_CHANNEL (3 channels) and FOUR_CHANNEL (4 channels).\n
	Possible type values are UNSIGNED_CHAR (aka UCHAR), SIGNED_CHAR (aka CHAR), 
	UNSIGNED_SHORT (aka USHORT), SIGNED_SHORT (aka SHORT), SIGNED_INT (aka INT), FLOAT and DOUBLE.
	
	If any other value is provided the constructor will throw std::runtime_error exception.

	@param size 2D repersentation of image's width and height.
	@param depth image depth.
	@param type internal image data type.
	*/
	ImageConstructInfo(const Size &size, const Shape3D::Depth &depth, const ImageDataType &type);
	/** @brief Transform constructor.

	Possible channel values are 0, 1, 3 and 4.\n
	Possible type values are UNSIGNED_CHAR (aka UCHAR), SIGNED_CHAR (aka CHAR),
	UNSIGNED_SHORT (aka USHORT), SIGNED_SHORT (aka SHORT), SIGNED_INT (aka INT), FLOAT and DOUBLE.

	If any other value is provided the constructor will throw std::runtime_error exception.

	@param size 2D repersentation of image's width and height.
	@param channels number of image channels.
	@param type internal image data type.
	*/
	ImageConstructInfo(const Size &size, size_t channels, const ImageDataType &type);
	/** @brief Transform constructor.

	Possible depth values are FLAT (0 channels), GRAYSCALE (1 channel), THREE_CHANNEL (3 channels) and FOUR_CHANNEL (4 channels).\n
	Possible type values are UNSIGNED_CHAR (aka UCHAR), SIGNED_CHAR (aka CHAR),
	UNSIGNED_SHORT (aka USHORT), SIGNED_SHORT (aka SHORT), SIGNED_INT (aka INT), FLOAT and DOUBLE.

	If any other value is provided the constructor will throw std::runtime_error exception.

	@param width image width.
	@param height image height.
	@param depth image depth.
	@param type internal image data type.
	*/
	ImageConstructInfo(size_t width, size_t height, const Shape3D::Depth &depth, const ImageDataType &type);
	/** @brief Transform constructor.

	Possible channel values are 0, 1, 3 and 4.\n
	Possible type values are UNSIGNED_CHAR (aka UCHAR), SIGNED_CHAR (aka CHAR),
	UNSIGNED_SHORT (aka USHORT), SIGNED_SHORT (aka SHORT), SIGNED_INT (aka INT), FLOAT and DOUBLE.

	If any other value is provided the constructor will throw std::runtime_error exception.

	@param width image width.
	@param height image height.
	@param channels number of image channels.
	@param type internal image data type.
	*/
	ImageConstructInfo(size_t width, size_t height, size_t channels, const ImageDataType &type);

	/** @brief Transform constructor.

	Please note, that only 1, 3, and 4 channel OpenCV-encodings are supported.\n
	For example, CV_8UC1 is supported, but CV_8UC2 is not.\n\n
	Passing unsupported enconding will result in throwing std::runtime_error exception.

	@param size 2D representation of image's width and height.
	@param cvType OpenCV-style int value, encoding internal data type and number of channels.
	*/
	ImageConstructInfo(const Size &size, int cvType);
	/** @brief Transform constructor.

	Please note, that only 1, 3, and 4 channel OpenCV-encodings are supported.\n
	For example, CV_8UC1 is supported, but CV_8UC2 is not.\n\n
	Passing unsupported enconding will result in throwing std::runtime_error exception.

	@param width image width.
	@param height image height.
	@param cvType OpenCV-style int value, encoding internal data type and number of channels.
	*/
	ImageConstructInfo(size_t width, size_t height, int cvType);

	/** @brief Copy constructor.
	*/
	ImageConstructInfo(const ImageConstructInfo &imageConstructInfo);

	/** @brief Assignment operator.
	*/
	ImageConstructInfo &operator=(const ImageConstructInfo &other);

	/** @brief Equality operator.

		Returns true if and only if all of the width, height, number of channels and internal data type of both ImageConstructInfos are correspondingly equal.
	*/
	bool operator == (const ImageConstructInfo &other) const;
	/** @brief Non-equality operator.

	Returns value opposite to equality operator's return value.
	*/
	bool operator != (const ImageConstructInfo &other) const;

	/** @brief returns 3D representation of ImageConstructInfo's width, height and depth.
	*/
	Shape3D shape3D() const;
	/** @brief returns ImageConstructInfo's depth.
	*/
	Shape3D::Depth depth() const;

	/** @brief returns 2D representation of ImageConstructInfo's width and height.
	*/
	Size size() const;

	/** @brief returns ImageConstructInfo's width.
	*/
	size_t width() const;
	/** @brief returns ImageConstructInfo's height.
	*/
	size_t height() const;
	/** @brief returns ImageConstructInfo's number of channels.
	*/
	size_t channels() const;

	/** @brief returns ImageConstructInfo's internal data type.
	*/
	ImageDataType imageDataType() const;

	/** @brief String representation.
	
	User-friendly string representation of ImageConstructInfo's width, height, number of channels and internal data-type.

	For example, ImageConstructInfo with width = 40, height = 39, channels = 1 and internal data type INT will have string 
	representation:
	@code
	ImageConstructInfo:
	ImageDataType: INT
	[[40x39]x1]
	@endcode
	*/
	std::string toString() const;

private:

	std::string imageDataTypeToString(const ImageDataType &type) const;
	
	Shape3D::Depth getDepthFromCVType(int cvType) const;
	ImageDataType getImageDataTypeFromCVType(int cvType) const;
};


}