#pragma once

#include <array>

class Point
{
public:
	int x, y;

	Point() : x(0), y(0) {}

	Point(int x, int y) : x(x), y(y) {}

	Point(const Point&p) : x(p.x), y(p.y) {}

	inline bool operator== (const Point& p) const;

	inline std::array<Point, 8> neighbours() const;
};