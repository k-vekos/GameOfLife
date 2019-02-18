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

	std::vector<Cell> aliveCells;

	const int maxThreads;

	sf::Vector2i worldSize;

	sf::Color getThreadColor(int index);

private:
	std::vector<uint8_t> world;

	std::vector<uint8_t> worldBuffer;	
};