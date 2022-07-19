#pragma once

#include <string>
#include <vector>

namespace CVUtils
{
class Color
{
public:

	enum class FORMAT {
		GRAY, BGR, RGB, BGRA, RGBA
	};

private:

	std::vector<float> m_channels;
	FORMAT m_format;

public:

	Color();
	Color(const std::vector<float> &channels, const FORMAT &format);
	Color(float c1, float c2 = .0f, float c3 = .0f, const FORMAT &format = FORMAT::BGR, float c4 = .0f);

	Color(const Color &other);

	bool operator == (const Color &other) const;

};

std::string toString(const Color::FORMAT &format);

}