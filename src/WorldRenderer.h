#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameOfLife.h"

class WorldRenderer
{
	public:
		WorldRenderer();
		~WorldRenderer();
		void render(sf::RenderWindow& window, GameOfLife& world);

	private:
		std::vector<sf::Vertex> m_cellVertexPoints;
		void addQuad(int gridX, int gridY, sf::Color color);
		void addBackgroundQuad(sf::Vector2f topLeft, sf::Vector2f bottomRight, sf::Color color);
		void renderBackground(sf::RenderWindow& window, GameOfLife& world);
		sf::Color darkenColor(sf::Color input);
};

