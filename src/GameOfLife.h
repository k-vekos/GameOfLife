#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class GameOfLife
{
public:
	GameOfLife(int sizeX, int sizeY);
	void doUpdate(sf::Vector2i startPos, sf::Vector2i endPos);
	virtual ~GameOfLife() = default;
	void update();
	std::vector<sf::Vector2i> getLiveCells();	
	void setCell(int x, int y, bool alive);

private:
	std::vector<std::vector<bool>> world;

protected:
	template<typename Func>
	void cellForEach(Func function);
};