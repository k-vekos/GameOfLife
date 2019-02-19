#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameOfLife.h"

class WorldRenderer
{
	public:
		WorldRenderer();

		~WorldRenderer();

		// Renders the given game to the given window.
		void render(sf::RenderWindow& window, GameOfLife& world);

	private:
		// Vertex points for the pending draw call.
		std::vector<sf::Vertex> m_vertexPoints;

		// Adds a cell-sized quad in the "grid position" specified.
		void addQuad(int gridX, int gridY, sf::Color color);

		// Adds a darker colored quad in the given coordinates.
		void addBackgroundQuad(sf::Vector2f topLeft, sf::Vector2f bottomRight, sf::Color color);

		// Renders the background colors which correspond to the thread ID and the cells they are updating.
		void renderBackgrounds(sf::RenderWindow& window, GameOfLife& world);

		// Returns a darker variant of the given color.
		sf::Color darkenColor(sf::Color input);
};

