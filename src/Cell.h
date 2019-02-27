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

	Cell(int x, int y, bool alive) : x(x), y(y), alive(alive) {}

	Cell(const Cell&p) : x(p.x), y(p.y) {}

	bool operator== (const Cell& p) const;

	std::array<Cell, 8> neighbors(sf::Vector2i worldSize) const;
};

namespace std
{
	template <>
	struct hash<Cell>
	{
		size_t operator()(const Cell& k) const
		{
			return hash<int>()(k.x * 3 + k.y * 5);
		}
	};
}