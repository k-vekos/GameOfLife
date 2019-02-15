#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace
{
	constexpr int WORLD_SIZE_X = 256;
	constexpr int WORLD_SIZE_Y = 256;
}

class GameOfLife
{
public:
	GameOfLife();
	virtual ~GameOfLife() = default;
	void render(sf::RenderWindow& window);
	void update();

private:
	bool world[WORLD_SIZE_X][WORLD_SIZE_Y] { false };
	void addQuad(int posX, int posY);
	void setCellColour(int x, int y, sf::Color color);
	std::vector<sf::Vertex> m_cellVertexPoints;
};