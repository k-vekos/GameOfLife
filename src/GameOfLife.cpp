#include "GameOfLife.h"
#include "Cell.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>

GameOfLife::GameOfLife(sf::Vector2i size) : worldSize(size), world(size.x * size.y, false), worldBuffer(world), maxThreads(std::thread::hardware_concurrency())
{
	aliveCells.reserve(size.x * size.y); // reserve space for worst-case (all cells are alive)

	int midX = worldSize.x / 2;
	int midY = worldSize.y / 2;

	// place an "acorn"
	getCell(midX + 0, midY + 0) = 1;
	getCell(midX + 1, midY + 0) = 1;
	getCell(midX + 4, midY + 0) = 1;
	getCell(midX + 5, midY + 0) = 1;
	getCell(midX + 6, midY + 0) = 1;
	getCell(midX + 3, midY + 1) = 1;
	getCell(midX + 1, midY + 2) = 1;

	/*
	// place a glider
	getCell(1, 3) = true;
	getCell(2, 4) = true;
	getCell(3, 2) = true;
	getCell(3, 3) = true;
	getCell(3, 4) = true;

	// place a glider at top-center
	int midX = std::floor(worldSize.x / 2);
	//getCell(std::floor(worldSize.x / 2), 0) = true;
	getCell(midX + 1, 3) = true;
	getCell(midX + 2, 4) = true;
	getCell(midX + 3, 2) = true;
	getCell(midX + 3, 3) = true;
	getCell(midX + 3, 4) = true;
	*/

	// place a glider
	/*world[world.size() - 10 + 1][world[0].size() - 10 + 3] = true;
	world[world.size() - 10 + 2][world[0].size() - 10 + 4] = true;
	world[world.size() - 10 + 3][world[0].size() - 10 + 2] = true;
	world[world.size() - 10 + 3][world[0].size() - 10 + 3] = true;
	world[world.size() - 10 + 3][world[0].size() - 10 + 4] = true;*/
}

uint8_t& GameOfLife::getCell(int x, int y)
{
	return world[y * worldSize.x + x];
}

sf::Vector2i GameOfLife::get2D(int index)
{
	int y = index / worldSize.x;
	int x = index % worldSize.x;
	return { x, y };
}

sf::Color GameOfLife::getThreadColor(int index)
{
	switch (index % 4) {
		case 0:
			return sf::Color::Red;
			break;
		case 1:
			return sf::Color::Green;
			break;
		case 2:
			return sf::Color::Blue;
			break;
		case 3:
			return sf::Color::Yellow;
			break;
	}
}

// Update the cells from position start (inclusive) to position end (exclusive).
std::vector<Cell> GameOfLife::doUpdate(int start, int end, int coreIdx)
{
	std::vector<Cell> aliveCells;
	aliveCells.reserve(end - start); // reserve space for worst case (all alive cells)

	for (int i = start; i < end; i++)
	{
		auto pos = get2D(i);

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
				int newX = (nX + pos.x + worldSize.x) % worldSize.x;
				int newY = (nY + pos.y + worldSize.y) % worldSize.y;

				aliveCount += getCell(newX, newY);
			}
		}

		// Evaluate game rules on current cell
		bool dies = aliveCount == 2 || aliveCount == 3;
		bool lives = aliveCount == 3;
		worldBuffer[i] = world[i] ? dies : lives;

		// if the cell's alive push it into the vector
		if (worldBuffer[i])
			aliveCells.push_back(Cell(pos, getThreadColor(coreIdx)));
	}

	return aliveCells;
}

void GameOfLife::update()
{
	// clear aliveCells cache
	aliveCells.clear();

	// divide the grid into horizontal slices
	int chunkSize = (worldSize.x * worldSize.y) / maxThreads;
	
	// split the work into threads
	std::vector<std::future<std::vector<Cell>>> asyncTasks;
	for (int i = 0; i < maxThreads; i++)
	{
		int start = i * chunkSize;

		int end;
		if (i == maxThreads - 1) // if this is the last thread, endPos will be set to cover remaining "height"
			end = worldSize.x * worldSize.y;
		else
			end = (i + 1) * chunkSize;

		asyncTasks.push_back(
			std::async(std::launch::async, [this, start, end, i] { return this->doUpdate(start, end, i); })
		);
	}
	
	// Wait until all async tasks are finished
	for (auto&& task : asyncTasks) { // TODO Why use 'auto&&'?
		auto aliveCellsPartial = task.get();
		aliveCells.insert(std::end(aliveCells), std::begin(aliveCellsPartial), std::end(aliveCellsPartial));
	}

	// apply updates
	world.swap(worldBuffer);
}

//std::vector<Cell> GameOfLife::getLiveCells()
//{
//	std::vector<Cell> liveCells;
//	liveCells.reserve(worldSize.x * worldSize.y); // reserve space for worst case (every cell is alive)
//
//	for (int i = 0; i < worldSize.x * worldSize.y; i++) {
//		auto pos = get2D(i);
//		if (world[i])
//			liveCells.push_back(Cell(sf::Vector2i(pos.x, pos.y), sf::Color::Red));
//	}
//
//	return liveCells;
//}

void GameOfLife::setCell(int x, int y, bool alive)
{
	// constrain x and y
	x = std::max(std::min(x, (int) worldSize.x - 1), 0);
	y = std::max(std::min(y, (int) worldSize.y - 1), 0);
	getCell(x, y) = alive;
	aliveCells.push_back(Cell(sf::Vector2i(x, y), sf::Color::White));
}