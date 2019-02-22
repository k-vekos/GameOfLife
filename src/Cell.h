#pragma once

#include <array>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class Cell
{
public:
	bool alive = false;

	sf::Color color = sf::Color::White;

	int x, y;

	Cell() : x(0), y(0) {}

	Cell(int x, int y) : x(x), y(y) {}

	Cell(const Cell&p) : x(p.x), y(p.y) {}

	inline bool operator== (const Cell& p) const;

	inline std::array<Cell, 8> neighbors(sf::Vector2i worldSize) const;
};

/*
#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct Cell
{
	sf::Vector2i position;
	sf::Color color;
};
*/