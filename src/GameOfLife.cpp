#include "GameOfLife.h"
#include <iostream>
#include <vector>
#include <math.h>

GameOfLife::GameOfLife()
{
	// initialize cell state and prepare the quads for rendering
	for (int x = 0; x < WORLD_SIZE_X; x++) {
		for (int y = 0; y < WORLD_SIZE_Y; y++) {
			// initialize cell state
			world[x][y] = false;
		}
	}


	// place a glider
	world[1][3] = true;
	world[2][4] = true;
	world[3][2] = true;
	world[3][3] = true;
	world[3][4] = true;

	// place a glider
	world[WORLD_SIZE_X - 10 + 1][WORLD_SIZE_Y - 10 + 3] = true;
	world[WORLD_SIZE_X - 10 + 2][WORLD_SIZE_Y - 10 + 4] = true;
	world[WORLD_SIZE_X - 10 + 3][WORLD_SIZE_Y - 10 + 2] = true;
	world[WORLD_SIZE_X - 10 + 3][WORLD_SIZE_Y - 10 + 3] = true;
	world[WORLD_SIZE_X - 10 + 3][WORLD_SIZE_Y - 10 + 4] = true;
}

void GameOfLife::update()
{
	// updates to be applied after all cells evaluated
	std::vector<sf::Vector2i> updates;

	// evaluate the state of every cell
	cellForEach([&](int x, int y)
	{
		// # of alive neighbors
		int aliveCount = 0;

		// check all 8 surrounding neighbors
		for (int nX = -1; nX <= 1; nX++) // nX = -1, 0, 1
		{
			for (int nY = -1; nY <= 1; nY++) // nY = -1, 0, 1
			{
				// make sure to skip the current cell!
				if (nX == 0 && nY == 0)
					continue;

				// wrap around to other side if neighbor would be outside world
				int newX = std::fmod(nX + x + WORLD_SIZE_X, WORLD_SIZE_X);
				int newY = std::fmod(nY + y + WORLD_SIZE_Y, WORLD_SIZE_Y);

				if (world[newX][newY])
					aliveCount++;
			}
		}


		/* evaluate game rules on current cell */

		// current cell location
		sf::Vector2i pos(x, y);

		switch (world[x][y]) // is current cell alive?
		{
			case true:
				if (aliveCount < 2 || aliveCount > 3)
				{
					updates.push_back(pos); // this cell will be toggled to dead
				}
				break;

			case false:
				if (aliveCount == 3)
				{
					updates.push_back(pos); // this cell will be toggled to alive
				}
				break;
		}
	});

	// apply updates to cell states
	for each (auto loc in updates)
	{
		// toggle the dead/alive state of every cell with a pending update
		world[loc.x][loc.y] = !world[loc.x][loc.y];
	}
}

std::vector<sf::Vector2i> GameOfLife::getLiveCells()
{
	std::vector<sf::Vector2i> liveCells;
	liveCells.reserve(WORLD_SIZE_X * WORLD_SIZE_Y); // reserve space for worst case (every cell is alive)

	cellForEach([&](int x, int y)
	{
		if (world[x][y])
			liveCells.push_back(sf::Vector2i(x, y));
	});

	return liveCells;
}

void GameOfLife::setCell(int x, int y, bool alive)
{
	// constrain x and y
	x = std::max(std::min(x, WORLD_SIZE_X - 1), 0);
	y = std::max(std::min(y, WORLD_SIZE_Y - 1), 0);

	world[x][y] = alive;
}

template<typename Func>
void GameOfLife::cellForEach(Func function)
{
	for (int x = 0; x < WORLD_SIZE_X; x++) {
		for (int y = 0; y < WORLD_SIZE_Y; y++) {
			function(x, y);
		}
	}
}
