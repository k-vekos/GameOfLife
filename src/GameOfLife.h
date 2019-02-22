#pragma once

#include "Cell.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

static const int Hash_Size = 8192;

size_t cellHash(const Cell & cell)
{
	return cell.x * 3 + cell.y * 5;
}

class GameOfLife
{
public:
	GameOfLife(sf::Vector2i size);

	// Returns a reference to the cell value at the given grid position.
	std::uint8_t & getCell(int x, int y);

	// Returns a vector of the given cell's grid position by it's cell index.
	sf::Vector2i get2D(int index) const;

	// Updates the state of the game world by one tick.
	void update();

	// Updates the state of the game world by one tick. (Multi-threaded)
	void updateMP();

	// Set the value of the cell at the given grid position to the given alive state.
	void setCell(int x, int y, bool alive);

	// Gets a collection of all the alive cells at the end of the update() call.
	auto const& getLivingCells() const { return aliveCells; }

	// Represents the width and height of the simulated world.
	const sf::Vector2i worldSize;

	// Returns a color to use for cells/backgrounds based on the thread ID #.
	static sf::Color getThreadColor(int index);

private:
	// A cache of all the alive cells at the end of the update() call.
	//std::vector<Cell> aliveCells = {};

	// A 1D representation of the 2D grid that is the world.
	std::vector<std::uint8_t> world;

	// A buffer where the next world state is prepared, swapped with world at end of update().
	std::vector<std::uint8_t> worldBuffer;

	// Collection of all of the alive cells at the end of the update() call.
	std::unordered_set<Cell> aliveCells;

	// Keeps track of cells which are neighbors of alive cells, and how many alive cells they're next to.
	//std::unordered_map<Cell, int> aliveCellNeighbors;
	std::unordered_map<Cell, int, decltype(&cellHash)> aliveCellNeighbors(Hash_Size, cellHash);

	// TODO L@@k here --> https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key/17017281#17017281
	// TODO and HERE --> https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

	void setDead(Cell cell);

	void setAlive(Cell cell);
};