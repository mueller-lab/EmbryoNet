#pragma once

#include "Point.hpp"
#include "Shape3D.hpp"
#include <opencv2/opencv.hpp>

#include <algorithm> 

namespace CVUtils
{

template<typename T>
class Bbox_
{
	std::vector<T> _xywh;

public:

	Bbox_<T>() {
		const std::size_t coordVecLength = 4;

		this->_xywh.resize(coordVecLength);

		this->_xywh[0] = T(0);
		this->_xywh[1] = T(0);
		this->_xywh[2] = T(0);
		this->_xywh[3] = T(0);
	}

	Bbox_<T>(T x, T y, T width, T height) {
		const std::size_t coordVecLength = 4;

		this->_xywh.resize(coordVecLength);

		this->_xywh[0] = x;
		this->_xywh[1] = y;
		this->_xywh[2] = width;
		this->_xywh[3] = height;
	}

	Bbox_<T>(const cv::Rect_<T> &rect) {
		*this = Bbox_<T>(rect.x, rect.y, rect.width, rect.height);
	}

	Bbox_<T>(const cv::Point_<T> &tl, const cv::Point_<T> &br) {
		cv::Rect rect(tl, br);
		*this = Bbox_<T>(rect);
	}

	Bbox_<T>(const Bbox_<T> &other) {
		this->_xywh = other._xywh;
	}

	Bbox_<T> &operator=(const Bbox_<T> &other) {
		this->_xywh = other._xywh;
		return *this;
	}

	bool operator == (const Bbox_<T> &other) const {

		for (size_t i = 0; i < other._xywh.size(); ++i) {
			if (this->_xywh[i] != other._xywh[i]) {
				return false;
			}
		}

		return true;
	}

	static Bbox_<T> fromXYWH(const std::vector<T> &xywh) {
		return Bbox_<T>(xywh[0], xywh[1], xywh[2], xywh[3]);
	}

	static Bbox_<T> fromXYAH(const std::vector<T> &xyah) {

		std::vector<T> xywh(xyah);

		if (xyah[3] == 0) {
			throw std::runtime_error(std::string("Height cannot equal to zero in xyah"));
		}

		xywh[2] = (xyah[2] * xyah[3]);

		xywh[0] -= xywh[2] / 2;
		xywh[1] -= xywh[3] / 2;
 
		return Bbox_<T>(xywh[0], xywh[1], xywh[2], xywh[3]);
	}

	static Bbox_<T> fromTLBR(const std::vector<T> &tlbr) {
		std::vector<T> xywh(tlbr);

		xywh[2] -= xywh[0];
		xywh[3] -= xywh[1];

		return Bbox_<T>(xywh[0], xywh[1], xywh[2], xywh[3]);
	}

	static Bbox_<T> fromTLBR(const Point_<T> &tl, const Point_<T> &br) {

		const std::size_t coordVecLength = 4;

		std::vector<T> tlbr(coordVecLength);

		tlbr[0] = tl.x();
		tlbr[1] = tl.y();
		tlbr[2] = br.x();
		tlbr[3] = br.y();

		return Bbox_<T>::fromTLBR(tlbr);
	}

	T x() const {
		return this->_xywh[0];
	}

	T y() const {
		return this->_xywh[1];
	}

	T width() const {
		return this->_xywh[2];
	}

	T height() const {
		return this->_xywh[3];
	}

	std::vector<T> xywh() const {
		return this->_xywh;
	}

	std::vector<T> xyah() const {

		std::vector<float> xyah(this->_xywh);

		xyah[0] += this->_xywh[2] / 2;
		xyah[1] += this->_xywh[3] / 2;

		xyah[2] = xyah[2] / xyah[3];

		return xyah;
	}

	std::vector<T> tlbr() const {
		return Point_<T>(this->_xywh[0], this->_xywh[1]);
	}

	Point_<T> tl() const {
		return Point_<T>(this->_xywh[0], this->_xywh[1]);
	}

	Point_<T> br() const {
		return Point_<T>(this->_xywh[0] + this->_xywh[2], this->_xywh[1] + this->_xywh[3]);
	}

	cv::Rect_<T> cvRect() const {
		return cv::Rect_<T>(int(this->_xywh[0]), int(this->_xywh[1]), int(this->_xywh[2]), int(this->_xywh[3]));
	}

	Size size() const {
		return Size(size_t(this->_xywh[2]), size_t(this->_xywh[3]));
	}

	operator cv::Rect() const {
		return this->cvRect();
	}

	T area() const {
		return this->_xywh[2] * this->_xywh[3];
	}

	double iou(const Bbox_<T> &other) const {

		T tunionArea = this->area() + other.area();

		if (tunionArea == T(0)) {
			return T(0);
		}

		Point_<T> tlInt(std::max(this->tl().x(), other.tl().x()), std::max(this->tl().y(), other.tl().y()));
		Point_<T> brInt(std::min(this->br().x(), other.br().x()), std::min(this->br().y(), other.br().y()));

		T intersectionArea = std::max(brInt.x() - tlInt.x(), T(0)) * std::max(brInt.y() - tlInt.y(), T(0));
		T unionArea = tunionArea - intersectionArea;

		return std::min(double(intersectionArea) / double(unionArea), 1.0);
	}

	std::string toString() const {
		std::string output;
		output = std::string("[") + std::to_string(this->x()) + ", " + std::to_string(this->y()) + std::string(" | [") + std::to_string(this->width()) + std::string(" x ") + std::to_string(this->height()) + std::string("] ]");
		return output;
	}
};

using Bbox = Bbox_<int>;
using BBox = Bbox;

using Bboxi = Bbox_<int>;
using BBoxi = Bboxi;
using Bboxf = Bbox_<float>;
using BBoxf = Bboxf;
using Bboxd = Bbox_<double>;
using BBoxd = Bboxd;

}
