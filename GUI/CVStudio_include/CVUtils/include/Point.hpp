#pragma once

#include <opencv2/opencv.hpp>

namespace CVUtils
{

template<typename T>
class Point_
{
    T _x, _y;

public:

	Point_<T>(): _x(T(0)), _y(T(0)) {}

    Point_<T>(T x, T y): _x(x), _y(y) {}
    Point_<T>(const Point_<T> &other): _x(other._x), _y(other._y) {}

    Point_<T>& operator=(const Point_<T> &other)
	{
		this->_x = other._x;
		this->_y = other._y;
		return *this;
	}

    bool operator==(const Point_<T> &other)
	{
		return ((this->_x == other._x) && (this->_y == other._y));
	}

    Point_<T>(cv::Point_<T> &cvPoint): _x(cvPoint.x), _y(cvPoint.y) {}

    operator cv::Point_<T>() const {
		return cv::Point_<T>(_x, _y); 
	}


    T x() const	{
		return this->_x;
	}

	T y() const {
		return this->_y;
	}

    std::string toString() const {
		return std::string("[") + std::to_string(this->_x) + std::string("x") + std::to_string(this->_y) + std::string("]");
	}
};

using Point = Point_<int>;

using Pointi = Point_<int>;
using Pointf = Point_<float>;
using Pointd = Point_<double>;

}

