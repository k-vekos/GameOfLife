#include "WorldRenderer.h"

WorldRenderer::WorldRenderer()
{
}


WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::addQuad(int gridX, int gridY, sf::Color color)
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

	topLeft.color = color;
	topRight.color = color;
	bottomLeft.color = color;
	bottomRight.color = color;

	m_cellVertexPoints.push_back(topLeft);
	m_cellVertexPoints.push_back(bottomLeft);
	m_cellVertexPoints.push_back(bottomRight);
	m_cellVertexPoints.push_back(topRight);
}

void WorldRenderer::addBackgroundQuad(sf::Vector2f topLeft, sf::Vector2f bottomRight, sf::Color color)
{
	sf::Vertex vTopLeft;
	sf::Vertex vTopRight;
	sf::Vertex vBottomLeft;
	sf::Vertex vBottomRight;

	vTopLeft.position = topLeft;
	vTopRight.position = { bottomRight.x, topLeft.y };
	vBottomLeft.position = { topLeft.x, bottomRight.y };
	vBottomRight.position = bottomRight;

	
	vTopLeft.color = color;
	vTopRight.color = color;
	vBottomLeft.color = color;
	vBottomRight.color = color;

	m_cellVertexPoints.push_back(vTopLeft);
	m_cellVertexPoints.push_back(vBottomLeft);
	m_cellVertexPoints.push_back(vBottomRight);
	m_cellVertexPoints.push_back(vTopRight);
}

void WorldRenderer::render(sf::RenderWindow & window, GameOfLife & game)
{
	// clear m_cellVertexPoints
	m_cellVertexPoints.clear();

	// draw backgrounds for "core zones"
	renderBackground(window, game);

	// populate m_cellVertexPoints
	for (auto cell : game.aliveCells)
	{
		addQuad(cell.position.x, cell.position.y, cell.color);
	}

	// draw quads to window
	window.draw(m_cellVertexPoints.data(), m_cellVertexPoints.size(), sf::Quads);
}

void WorldRenderer::renderBackground(sf::RenderWindow & window, GameOfLife & world)
{
	int cellsPerCore = world.worldSize.x * world.worldSize.y / world.maxThreads;

	// first draw the background color of the final core index
	addBackgroundQuad(
		sf::Vector2f(0, 0),
		sf::Vector2f(world.worldSize.x, world.worldSize.y),
		darkenColor(world.getThreadColor(world.maxThreads - 1))
	);

	// draw the remaining core background colors on top, in reverse order
	for (int i = world.maxThreads - 2; i >= 0; i--) {
		auto end = world.get2D(cellsPerCore * (i + 1));
		addBackgroundQuad(
			sf::Vector2f(0, 0),
			sf::Vector2f(world.worldSize.x, end.y),
			darkenColor(world.getThreadColor(i))
		);
	}
}

sf::Color WorldRenderer::darkenColor(sf::Color input)
{
	return sf::Color(input.r / 3, input.g / 3, input.b / 3);
}