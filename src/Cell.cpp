#include "Cell.h"

bool Cell::operator==(const Cell & other) const
{
	return x == other.x && y == other.y;
}

std::array<Cell, 8> Cell::neighbors(sf::Vector2i worldSize) const
{
	std::array<Cell, 8> neighborCells;

	// check all 8 neighbors
	int i = 0;
	for (int nX = -1; nX <= 1; ++nX) {
		for (int nY = -1; nY <= 1; ++nY) {
			// skip the current cell
			if (nX == 0 && nY == 0) continue;

			// wrap around to other side if neighbor would be outside world
			int newX = (nX + x + worldSize.x) % worldSize.x;
			int newY = (nY + y + worldSize.y) % worldSize.y;

			neighborCells[i] = Cell(newX, newY);
			i++;
		}
	}

	return neighborCells;
}

