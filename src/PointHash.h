#pragma once

#include "Point.h"

typedef uint32_t HashType;

class PointHash
{
public:
	inline HashType operator() (Point p) const
	{
		return (HashType)(p.x * 3 + p.y * 5);
	}
};