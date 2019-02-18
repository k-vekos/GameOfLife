#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Cell.h"

class GameOfLife
{
public:
	GameOfLife(sf::Vector2i size);
	uint8_t & getCell(int x, int y);
	sf::Vector2i get2D(int i);	
	void doUpdate(int start, int end);
	virtual ~GameOfLife() = default;
	void update();
	std::vector<Cell> GameOfLife::doUpdate(int start, int end, int coreIdx);
	void setCell(int x, int y, bool alive);
	std::vector<Cell> aliveCells;
	const int maxThreads;
	sf::Vector2i worldSize;
	sf::Color getThreadColor(int index);

private:
	std::vector<uint8_t> world;
	std::vector<uint8_t> worldBuffer;	
};