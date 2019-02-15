#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class GameOfLife
{
public:
	GameOfLife();
	virtual ~GameOfLife() = default;
	void update();
	std::vector<sf::Vector2i> getLiveCells();
	const static int WORLD_SIZE_X = 256;
	const static int WORLD_SIZE_Y = 256;

private:
	bool world[WORLD_SIZE_X][WORLD_SIZE_Y] { false };

protected:
	template<typename Func>
	void cellForEach(Func function);
};