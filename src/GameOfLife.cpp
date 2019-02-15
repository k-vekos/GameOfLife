#include "GameOfLife.h"
#include <iostream>
#include <vector>

GameOfLife::GameOfLife()
{
	// initialize cell state and prepare the quads for rendering
	for (int x = 0; x < WORLD_SIZE_X; x++) {
		for (int y = 0; y < WORLD_SIZE_Y; y++) {
			// initialize cell state
			world[x][y] = false;

			// prepare quad
			//addQuad(x, y);
		}
	}


	// place two live cells
	world[0][0] = true;
	world[WORLD_SIZE_X / 2][WORLD_SIZE_Y / 2] = true;
}

void GameOfLife::update()
{
	// update every cell
	/*cellForEach([&](int x, int y)
	{
	});*/
}

std::vector<std::pair<int, int>> GameOfLife::getLiveCells()
{
	std::vector<std::pair<int, int>> liveCells;
	liveCells.reserve(WORLD_SIZE_X * WORLD_SIZE_Y); // reserve space for worst case (every cell is alive)

	cellForEach([&](int x, int y)
	{
		if (world[x][y])
			liveCells.push_back(std::make_pair(x, y));
	});

	return liveCells;
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
