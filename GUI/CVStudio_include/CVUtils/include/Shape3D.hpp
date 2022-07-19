#pragma once

#include <string>
#include <opencv2/opencv.hpp>

/** @file */
namespace CVUtils
{

/** @brief 2D representation of image's width and height.

It has the same meaning as cv::Size, but, unlike it, provides encapsulation by not allowing explicit,
uncontrolled access to the internal width and height values.
*/

class Size
{
	size_t _width, _height;

public:

	/** @brief Default constructor.

	Default constructor sets width and height to 0.\n
	It is not intended to be used in normal situations.
	*/
	Size();
	/** @brief Transform constructor.

	@param width image's width.
	@param height image's height.
	*/
	Size(size_t width, size_t height);

	/** @brief Transform constructor.

	@param size cv::Size.
	*/
	Size(const cv::Size &size);

	/** @brief Copy constructor.
	*/
	Size(const Size &size);

	/** @brief Assignment operator.
	*/
	Size & operator = (const Size &other);
	
	/** @brief Converts to cv::Size.
	*/
	operator cv::Size() const;

	/** @brief Equality operator.

	Returns true if and only if all of the width, height of both Sizes are correspondingly equal.
	*/
	bool operator == (const Size &other) const;
	/** @brief Non-equality operator.

	Returns value opposite to equality operator's return value.
	*/
	bool operator != (const Size &other) const;

	/** @brief returns Size's width.
	*/
	size_t width() const;
	/** @brief returns Size's height.
	*/
	size_t height() const;

	/** @brief Rectangular area.
		
	Return value is the area of the rectangle with dimensions equal to Size object's width and height.
	*/
	size_t area() const;

	/** @brief String representation.

	User-friendly string representation of Size's width and height.
	For example, Size with width = 200, height = 149 will have string 
	representation:
	@code
	[200x149]
	@endcode
	*/
	std::string toString() const;
};

/** @brief 3D representation of image's width, height and number of channels.
*/
class Shape3D
{

public:

	/** @brief Depth of image

		It has the same meaning as number of channels, but, unlinke explicit setting of number of channels, 
		Depth usage allows to get rid of "magic numbers" and stick to the only allowed values.
	*/
	enum class Depth
	{
		/** @brief Flat image. It does not have any depth at all and usually not needed. channels() == 0. 
		*/
		FLAT = 0, 
		/** @brief Grayscale image: channels() == 1.
		*/
		GRAYSCALE = 1, 
		/** @brief Three channel, color image: channels() == 3.
		*/
		THREE_CHANNEL = 3, 
		/** @brief Four channel, alpha channel-containing image: channels() == 4.
		*/
		FOUR_CHANNEL = 4, 
		/** @brief No other channels number is supported.
		*/
		DEPTH_NONE = -1
	};

private:

	Size _size;
	Depth _depth;

public:

	/** @brief Default constructor.

	Default constructor sets width, height and number of channels to 0.\n
	It is not intended to be used in normal situations.
	*/
	Shape3D();

	/** @brief Transform constructor.

	Possible depth values are FLAT (0 channels), GRAYSCALE (1 channel), THREE_CHANNEL (3 channels) and FOUR_CHANNEL (4 channels).\n
	
	If any other value is provided the constructor will throw std::runtime_error exception.

	@param size 2D repersentation of image's width and height.
	@param depth image depth.
	*/
	Shape3D(const Size &size, Depth depth);
	/** @brief Transform constructor.

	Default channels number is 1. \n
	Possible channel values are 0, 1, 3 and 4.\n

	If any other value is provided the constructor will throw std::runtime_error exception.

	@param size 2D repersentation of image's width and height.
	@param channels number of image channels.
	*/
	Shape3D(const Size &size, size_t channels = 1);
	/** @brief Transform constructor.

	Possible depth values are FLAT (0 channels), GRAYSCALE (1 channel), THREE_CHANNEL (3 channels) and FOUR_CHANNEL (4 channels).\n

	If any other value is provided the constructor will throw std::runtime_error exception.

	@param width image width.
	@param height image height.
	@param depth image depth.
	*/
	Shape3D(size_t width, size_t height, Depth depth);
	/** @brief Transform constructor.

	Default channels number is 1. \n
	Possible channel values are 0, 1, 3 and 4.\n

	If any other value is provided the constructor will throw std::runtime_error exception.

	@param width image width.
	@param height image height.
	@param channels number of image channels.
	*/
	Shape3D(size_t width, size_t height, size_t channels = 1);
	
	/** @brief Copy constructor.
	*/
	Shape3D(const Shape3D &shape3D);

	/** @brief Assignment operator.
	*/
	Shape3D& operator = (const Shape3D &other);

	/** @brief Equality operator.

		Returns true if and only if all of the width, height, number of channels of both Shape3Ds are correspondingly equal.
	*/
	bool operator == (const Shape3D &other) const;
	/** @brief Non-equality operator.

	Returns value opposite to equality operator's return value.
	*/
	bool operator != (const Shape3D &other) const;

	/** @brief returns 2D representation of Shape3D's width and height.
	*/
	Size size() const;
	/** @brief returns Shape3D's width.
	*/
	size_t width() const;
	/** @brief returns Shape3D's height.
	*/
	size_t height() const;
	
	/** @brief returns Shape3D's depth.
	*/
	Depth depth() const;
	/** @brief returns Shape3D's number of channels.
	*/
	size_t channels() const;

	/** @brief Cuboid volume.
	
	Return value is the volume of a rectangular cuboid with dimensions equal to Shape3D object's width, height and number of channels.
	*/
	size_t volume() const;

	/** @brief String representation.

	User-friendly string representation of Shape3D's width, height and number of channels.

	For example, Shape3D with width = 401, height = 720 and channels = 4 will have string
	representation:
	@code
	[[401x720]x4]
	@endcode
	*/
	std::string toString() const;

private:

	bool checkDepthValidity(const Depth &depth) const;
};

}