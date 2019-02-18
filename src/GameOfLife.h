#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class GameOfLife
{
public:
	GameOfLife(sf::Vector2i size);
	uint8_t & getCell(int x, int y);
	sf::Vector2i get2D(int i);
	void doUpdate(int start, int end);
	virtual ~GameOfLife() = default;
	void update();
	std::vector<sf::Vector2i> getLiveCells();	
	void setCell(int x, int y, bool alive);

private:
	std::vector<uint8_t> world;
	std::vector<uint8_t> worldBuffer;
	sf::Vector2i worldSize;
};