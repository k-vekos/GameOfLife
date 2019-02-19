#pragma once

#include <SFML/Graphics.hpp>

class Cell
{
public:
	Cell(sf::Vector2i position, sf::Color color);
	~Cell();
	sf::Vector2i position;
	sf::Color color;
};
