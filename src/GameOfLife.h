#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class GameOfLife
{
public:
	GameOfLife();
	virtual ~GameOfLife() = default;
	void render(sf::RenderWindow& window);
	void update();

private:
	bool world[512][512] { false };
	void addQuad(int posX, int posY);
	void setCellColour(int x, int y, sf::Color color);
	std::vector<sf::Vertex> m_cellVertexPoints;
};