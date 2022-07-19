#pragma once

/** @file */

#include "Image.hpp"

namespace CVUtils
{

namespace IP 
{

/** @brief Base class of image processing algorithms in CVUtils.

The object of this class is constructed by the image to be processed. The results of image processing operations might be chained.
The underlying image storage type in this class is CVUtils::Image.

It is assumed, that image processing classes will be a virtual public subclass of this class and follow the chaining technique with the underlying
CVUtils::Image as a transformation data.

For example, consider usage of ShapeType image processing class, which is a subclass of ImageProcessor.
Output of any operation may implicitly be transformed to CVUtils::Image.\n
For example, assuming img is a variable of either cv::Mat or CVUtils::Image type:
@code
img = ShapeType(img).reshape3D(1920, 1080, 3).convertType(ImageDataType::UNSIGNED_CHAR);
@endcode
1) reshapes the img variable to have width=1920, height=1080, number of channels = 3;\n
2) converts internal data type of the image to UNSIGNED_CHAR.

*/

class ImageProcessor
{
public:
	/** @brief Transform constructor.

	The mat parameter is internally transformed to CVUtils::Image, which is underlying image storage type in this class.

	@param mat cv::Mat representation of the image to be processed.
	*/
	ImageProcessor(const cv::Mat &mat);

	/** @brief Transform constructor.

	@param img CVUtils::Image representation of the image to be processed.
	*/
	ImageProcessor(const CVUtils::Image &img);

	/** @brief Convert to CVUtils::Image.

	This operator is used to save the results of image processing operations in a form of CVUtils::Image.
	*/
	operator Image() const;

	/** @brief Convert to cv::Mat.

	This operator is used to save the results of image processing operations in a form of cv::Mat.
	*/
	operator cv::Mat() const;
    
protected:
    
    Image img;
};

}
}
