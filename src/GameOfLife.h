#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class GameOfLife
{
public:
	GameOfLife(int sizeX, int sizeY);
	uint8_t & getCell(int x, int y);
	void doUpdate(int start, int end);
	virtual ~GameOfLife() = default;
	void update();
	std::vector<sf::Vector2i> getLiveCells();	
	void setCell(int x, int y, bool alive);

private:
	std::vector<uint8_t> world;

protected:
	template<typename Func>
	void cellForEach(Func function);
};