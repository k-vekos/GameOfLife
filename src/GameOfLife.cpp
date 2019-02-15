#include "GameOfLife.h"
#include <iostream>

GameOfLife::GameOfLife()
{
	// initialize cell state and prepare the quads for rendering
	for (int x = 0; x < 512; x++) {
		for (int y = 0; y < 512; y++) {
			// initialize cell state
			world[x][y] = false;

			// prepare quad
			addQuad(x, y);
		}
	}


	// place two live cells
	world[0][0] = true;
	world[256][256] = true;
}

void GameOfLife::render(sf::RenderWindow & window)
{
	window.draw(m_cellVertexPoints.data(), m_cellVertexPoints.size(), sf::Quads);
}

void GameOfLife::update()
{
	// update every cell
	for (int x = 0; x < 512; x++) {
		for (int y = 0; y < 512; y++) {
			if (world[x][y]) {
				setCellColour(x, y, sf::Color::White);
				//std::cout << "x=" << x << "; y=" << y << "; alive=" << true << std::endl;
			}
			else {
				setCellColour(x, y, sf::Color::Black);
				//std::cout << "x=" << x << "; y=" << y << "; alive=" << false << std::endl;
			}
		}
	}
}

void GameOfLife::addQuad(int gridX, int gridY)
{
	sf::Vertex topLeft;
	sf::Vertex topRight;
	sf::Vertex bottomLeft;
	sf::Vertex bottomRight;

	float gridXFloat = gridX * 1.0f;
	float gridYFloat = gridY * 1.0f;

	topLeft.position =		{ gridXFloat, gridYFloat };
	topRight.position =		{ gridXFloat + 1, gridYFloat };
	bottomLeft.position =	{ gridXFloat, gridYFloat + 1 };
	bottomRight.position =	{ gridXFloat + 1, gridYFloat + 1 };

	m_cellVertexPoints.push_back(topLeft);
	m_cellVertexPoints.push_back(bottomLeft);
	m_cellVertexPoints.push_back(bottomRight);
	m_cellVertexPoints.push_back(topRight);
}

void GameOfLife::setCellColour(int x, int y, sf::Color color)
{
	auto index = (y * 512 + x) * 4;
	for (int i = 0; i < 4; i++)
	{
		m_cellVertexPoints[index + i].color = color;
	}
}