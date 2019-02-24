#include "WorldRenderer.h"

#include <omp.h>

void WorldRenderer::addQuad(const Cell& cell)
{

	sf::Vector2f topLeft{ cell.position.x * 1.0f, cell.position.y * 1.0f };
	sf::Vector2f bottomRight{ topLeft.x + 1, topLeft.y + 1 };
	addQuad(topLeft, bottomRight, cell.color);
}

void WorldRenderer::addQuad(sf::Vector2f topLeft, sf::Vector2f bottomRight, sf::Color color)
{
	auto topRight = topLeft;
	auto bottomLeft = bottomRight;
	std::swap(topRight.x, bottomLeft.x);

	m_vertexPoints.emplace_back(topLeft, color);
	m_vertexPoints.emplace_back(bottomLeft, color);
	m_vertexPoints.emplace_back(bottomRight, color);
	m_vertexPoints.emplace_back(topRight, color);
}

void WorldRenderer::render(sf::RenderWindow & window, GameOfLife & game)
{
	// clear m_cellVertexPoints
	m_vertexPoints.clear();

	// draw backgrounds for "core zones"
	renderBackgrounds(game);

	// populate m_cellVertexPoints
	for (auto const& cell : game.getLivingCells()) {
		addQuad(cell);
	}

	// draw quads to window
	window.draw(m_vertexPoints.data(), m_vertexPoints.size(), sf::Quads);
}

void WorldRenderer::renderBackgrounds(GameOfLife & world)
{
	auto const maxThreads = omp_get_max_threads();
	auto const threadHeight = world.worldSize.y / maxThreads;
	
	for (int i = 0; i < maxThreads; ++i) {
		sf::Vector2f topLeft{ 0, 1.f * i * threadHeight };
		sf::Vector2f bottomRight{ 1.f * world.worldSize.x + 1, topLeft.y + 1.f * world.worldSize.y / maxThreads + 1 };
		addQuad(topLeft, bottomRight, darkenColor(world.getThreadColor(i)));
	}
}

sf::Color WorldRenderer::darkenColor(sf::Color input)
{
	return sf::Color(input.r / 4, input.g / 4, input.b / 4);
}