#include "Point.h"

inline bool Point::operator==(const Point & p) const
{
	return x == p.x && y == p.y;
}

inline std::array<Point, 8> Point::neighbours() const
{
	std::array<Point, 8> result = {
			Point(x - 1, y - 1),
			Point(x - 1, y),
			Point(x - 1, y + 1),
			Point(x, y - 1),
			Point(x, y + 1),
			Point(x + 1, y - 1),
			Point(x + 1, y),
			Point(x + 1, y + 1)
	};
	return result;
}
