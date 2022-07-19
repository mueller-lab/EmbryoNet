#pragma once

#include <opencv2/opencv.hpp>

#include "Color.hpp"
#include "ImageConstructInfo.hpp"

/** @file */
namespace CVUtils
{

	
class Image
{
	cv::Mat mat;
	Color::FORMAT format;

public:

	Image();
	Image(const cv::Mat &mat);
	Image(const std::string &fileName);

	void load(const std::string &fileName);
	void save(const std::string &fileName) const;

	ImageConstructInfo imageConstructInfo() const;

	ImageDataType imageDataType() const;

	Shape3D shape3D() const;

	Size size() const;
	Shape3D::Depth depth() const;

	size_t width() const;
	size_t height() const;
	
	size_t rows() const;
	size_t cols() const;
	
	size_t channels() const;
	
	bool empty() const;

	Color::FORMAT colorFormat() const;

	void clear();
	void show(const std::string windowName = std::string(""), int delay = 0);

	operator cv::Mat() const;
    
    cv::Mat& matRef();
	
	void swapChannels();
	void updateFormat();

	Image clone() const;

};

} 
