#include "GameOfLife.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <thread>
#include <mutex>

std::mutex updateListLock;
std::vector<sf::Vector2i> pendingUpdates;
sf::Vector2i worldSize;

GameOfLife::GameOfLife(int sizeX, int sizeY)
{
	worldSize = sf::Vector2i(sizeX, sizeY);

	// initialize world to specified size, all starting as dead
	world = std::vector<uint8_t>(sizeX * sizeY, 0);

	// reserve space for worst case (every cell needs to be updated)
	pendingUpdates.reserve(sizeX * sizeY);

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

// Update the cells from position start (inclusive) to position end (exclusive).
void GameOfLife::doUpdate(int start, int end)
{
	for (int i = start; i < end; i++)
	{
		// TODO Try to calculate 2D wrapping without this?
		int y = std::floor(i / worldSize.x);
		int x = i % worldSize.x;

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
				int newX = (nX + x + worldSize.x) % worldSize.x;
				int newY = (nY + y + worldSize.y) % worldSize.y;

				aliveCount += getCell(newX, newY);
			}
		}

		/* evaluate game rules on current cell */

		sf::Vector2i pos(x, y); // current cell location
		switch (getCell(x, y)) // is current cell alive?
		{
		case true:
			if (aliveCount < 2 || aliveCount > 3)
			{
				std::lock_guard<std::mutex> lock(updateListLock);
				pendingUpdates.push_back(pos); // this cell will be toggled to dead
			}
			break;

		case false:
			if (aliveCount == 3)
			{
				std::lock_guard<std::mutex> lock(updateListLock);
				pendingUpdates.push_back(pos); // this cell will be toggled to alive
			}
			break;
		}
	}
}

void GameOfLife::update()
{
	unsigned maxThreads = std::thread::hardware_concurrency();
	//std::cout << maxThreads << std::endl;

	// divide the grid into horizontal slices
	//int chunkHeight = worldSize.y / maxThreads;
	int chunkSize = (worldSize.x * worldSize.y) / maxThreads;

	// split the work into threads
	std::vector<std::thread> threads;
	for (int i = 0; i < maxThreads; i++)
	{
		int start = i * chunkSize;

		int end;
		if (i == maxThreads - 1) // if this is the last thread, endPos will be set to cover remaining "height"
			end = worldSize.x * worldSize.y;
		else
			end = (i + 1) * chunkSize;

		std::thread t([this, start, end] {
			this->doUpdate(start, end);
		});

		//std::cout << "thread #" << i << ": cells " << start << "-" << end << std::endl;

		threads.push_back(std::move(t));
	}

	for (std::thread & t : threads) {
		if (t.joinable())
			t.join();
	}

	// apply updates to cell states
	for each (auto loc in pendingUpdates)
	{
		// toggle the dead/alive state of every cell with a pending update
		getCell(loc.x, loc.y) = !getCell(loc.x, loc.y);
	}

	// clear updates
	pendingUpdates.clear();
}

std::vector<sf::Vector2i> GameOfLife::getLiveCells()
{
	std::vector<sf::Vector2i> liveCells;
	liveCells.reserve(worldSize.x * worldSize.y); // reserve space for worst case (every cell is alive)

	cellForEach([&](int x, int y)
	{
		if (getCell(x, y))
			liveCells.push_back(sf::Vector2i(x, y));
	});

	return liveCells;
}

void GameOfLife::setCell(int x, int y, bool alive)
{
	// constrain x and y
	x = std::max(std::min(x, (int) worldSize.x - 1), 0);
	y = std::max(std::min(y, (int) worldSize.y - 1), 0);
	getCell(x, y) = alive;
}

template<typename Func>
void GameOfLife::cellForEach(Func function)
{
	for (int x = 0; x < worldSize.x; x++) {
		for (int y = 0; y < worldSize.y; y++) {
			function(x, y);
		}
	}
}
