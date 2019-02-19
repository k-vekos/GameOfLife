#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Cell.h"

class GameOfLife
{
public:
	GameOfLife(sf::Vector2i size);

	virtual ~GameOfLife() = default;

	// Returns a reference to the cell value at the given grid position.
	uint8_t & getCell(int x, int y);

	// Returns a vector of the given cell's grid position by it's cell index.
	sf::Vector2i get2D(int index);

	// Updates the state of the game world by one tick.
	void update();

	// Update the cells from position start (inclusive) to position end (exclusive).
	std::vector<Cell> GameOfLife::doUpdate(int start, int end, int coreIdx);

	// Set the value of the cell at the given grid position to the given alive state.
	void setCell(int x, int y, bool alive);

	// A cache of all the alive cells at the end of the update() call.
	std::vector<Cell> aliveCells;

	// The maximum amount of threads to be used for update().
	const int maxThreads;

	// Represents the width and height of the simulated world.
	sf::Vector2i worldSize;

	// Returns a color to use for cells/backgrounds based on the thread ID #.
	sf::Color getThreadColor(int index);

private:
	// A 1D representation of the 2D grid that is the world.
	std::vector<uint8_t> world;

	// A buffer where the next world state is prepared, swapped with world at end of update().
	std::vector<uint8_t> worldBuffer;	
};