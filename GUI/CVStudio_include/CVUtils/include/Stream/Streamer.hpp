#pragma once

#include "Image.hpp"

/** @file */

namespace CVUtils
{

class Streamer
{
public:
	virtual Streamer& operator>>(Image &img) = 0;

	virtual ~Streamer() {}

	virtual bool isOpened() const = 0;
	virtual void close() = 0;
};

}