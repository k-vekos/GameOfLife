#include "WorldRenderer.h"



WorldRenderer::WorldRenderer()
{
}


WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::addQuad(int gridX, int gridY)
{
	sf::Vertex topLeft;
	sf::Vertex topRight;
	sf::Vertex bottomLeft;
	sf::Vertex bottomRight;

	float gridXFloat = gridX * 1.0f;
	float gridYFloat = gridY * 1.0f;

	topLeft.position = { gridXFloat, gridYFloat };
	topRight.position = { gridXFloat + 1, gridYFloat };
	bottomLeft.position = { gridXFloat, gridYFloat + 1 };
	bottomRight.position = { gridXFloat + 1, gridYFloat + 1 };

	m_cellVertexPoints.push_back(topLeft);
	m_cellVertexPoints.push_back(bottomLeft);
	m_cellVertexPoints.push_back(bottomRight);
	m_cellVertexPoints.push_back(topRight);
}

void WorldRenderer::render(sf::RenderWindow & window, GameOfLife & game)
{
	// clear m_cellVertexPoints
	m_cellVertexPoints.clear();

	// populate m_cellVertexPoints
	auto aliveLocs = game.getLiveCells();
	for each (auto loc in aliveLocs)
	{
		addQuad(loc.x, loc.y);
	}

	// draw quads to window
	window.draw(m_cellVertexPoints.data(), m_cellVertexPoints.size(), sf::Quads);
}