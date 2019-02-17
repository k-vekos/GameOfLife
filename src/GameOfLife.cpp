#include "GameOfLife.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <thread>
#include <mutex>

std::mutex updateListLock;
std::vector<sf::Vector2i> pendingUpdates;

GameOfLife::GameOfLife(int sizeX, int sizeY)
{
	// initialize world to specified size, all starting as dead
	world = std::vector<std::vector<bool>>(sizeX, std::vector<bool>(sizeY, false));
	//vector<vector<int> >  matrix(4, vector<int>(4,5));

	// place a glider
	world[1][3] = true;
	world[2][4] = true;
	world[3][2] = true;
	world[3][3] = true;
	world[3][4] = true;

	// place a glider
	/*world[world.size() - 10 + 1][world[0].size() - 10 + 3] = true;
	world[world.size() - 10 + 2][world[0].size() - 10 + 4] = true;
	world[world.size() - 10 + 3][world[0].size() - 10 + 2] = true;
	world[world.size() - 10 + 3][world[0].size() - 10 + 3] = true;
	world[world.size() - 10 + 3][world[0].size() - 10 + 4] = true;*/
}

void GameOfLife::doUpdate(sf::Vector2i startPos, sf::Vector2i endPos)
{
	for (int x = startPos.x; x < endPos.x; x++) {
		for (int y = startPos.y; y < endPos.y; y++) {
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
					int newX = std::fmod(nX + x + world.size(), world.size());
					int newY = std::fmod(nY + y + world[0].size(), world[0].size());

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
}

void GameOfLife::update()
{
	unsigned maxThreads = std::thread::hardware_concurrency();

	// divide the grid into horizontal slices
	int chunkHeight = world[0].size() / maxThreads;

	// split the work into threads
	std::vector<std::thread> threads;
	for (int i = 0; i < maxThreads; i++)
	{
		sf::Vector2i startPos(0, i * chunkHeight);

		sf::Vector2i endPos;
		if (i == maxThreads - 1) // if this is the last thread, endPos will be set to cover remaining "height"
			endPos = sf::Vector2i(world.size() - 1, world[0].size() - 1);
		else
			endPos = sf::Vector2i(world.size() - 1, (i + 1) * chunkHeight - 1);

		std::thread t([this, startPos, endPos] {
			this->doUpdate(startPos, endPos);
		});

		std::cout << "thread #" << i << ": start_x=" << startPos.x << " start_y=" << startPos.y;
		std::cout << " end_x=" << endPos.x << " end_y=" << endPos.y << std::endl;

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
		world[loc.x][loc.y] = !world[loc.x][loc.y];
	}

	// clear updates
	pendingUpdates.clear();
}

std::vector<sf::Vector2i> GameOfLife::getLiveCells()
{
	std::vector<sf::Vector2i> liveCells;
	liveCells.reserve(world.size() * world[0].size()); // reserve space for worst case (every cell is alive)

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
	x = std::max(std::min(x, (int) world.size() - 1), 0);
	y = std::max(std::min(y, (int) world[0].size() - 1), 0);

	world[x][y] = alive;
}

template<typename Func>
void GameOfLife::cellForEach(Func function)
{
	for (int x = 0; x < world.size(); x++) {
		for (int y = 0; y < world[9].size(); y++) {
			function(x, y);
		}
	}
}
