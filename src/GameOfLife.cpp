#include "GameOfLife.h"

#include <omp.h>

#include <array>

GameOfLife::GameOfLife(sf::Vector2i size) : worldSize(size), world(size.x * size.y, false), worldBuffer(world)
{
	aliveCellNeighbors.reserve(Hash_Size); // reserve space for worst-case (all cells are in the map)

	// place an "acorn"
	int midX = worldSize.x / 2;
	int midY = worldSize.y / 2;	
	getCell(midX + 0, midY + 0) = 1;
	getCell(midX + 1, midY + 0) = 1;
	getCell(midX + 4, midY + 0) = 1;
	getCell(midX + 5, midY + 0) = 1;
	getCell(midX + 6, midY + 0) = 1;
	getCell(midX + 3, midY + 1) = 1;
	getCell(midX + 1, midY + 2) = 1;

	// build a list of all alive cells and a list of each one's neighbors
	for (int i = 0; i < worldSize.x * worldSize.y; ++i) {
		auto pos = get2D(i);

		if (world[i]) {
			// get a new Cell object for this alive cell
			Cell aliveCell{ pos.x, pos.y, true };

			// add to alive cells list
			aliveCells.insert(aliveCell);

			// add 1 to the count of all of its neighbors in the alive cells neighbors list
			for (const Cell& n : aliveCell.neighbors(worldSize)) {
				aliveCellNeighbors[n]++;
			}
		}
	}
}

std::uint8_t& GameOfLife::getCell(int x, int y)
{
	return world[y * worldSize.x + x];
}

sf::Vector2i GameOfLife::get2D(int index) const
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

	return sf::Color::White;
}

void GameOfLife::update()
{
	std::vector<Cell> toDie;
	std::vector<Cell> toLive;
	for (auto& c : aliveCells) {
		int liveCount = aliveCellNeighbors[c];
		if (liveCount < 2 || liveCount > 3) {
			toDie.push_back(c);
		}
	}

	for (auto& c : aliveCellNeighbors) {
		if (aliveCells.find(c.first) != aliveCells.end()) // is this cell alive?
			continue; // if so skip because we already updated aliveCells

		if (aliveCellNeighbors[c.first] == 3) {
			toLive.push_back(c.first);
		}
	}

	for (const Cell& deadCell : toDie) {
		setDead(deadCell);
	}

	for (const Cell& liveCell : toLive) {
		setAlive(liveCell);
	}
}

void GameOfLife::updateMP()
{
	std::vector<Cell> toDie;
	std::vector<Cell> toLive;

#pragma omp parallel
	{
		// private, per-thread variables
		std::vector<Cell> myToDie;
		std::vector<Cell> myToLive;

#pragma omp for
		for (int i = 0; i < aliveCells.size(); i++) {
			auto it = aliveCells.begin();
			std::advance(it, i);

			int liveCount = aliveCellNeighbors[*it];
			if (liveCount < 2 || liveCount > 3) {
				myToDie.push_back(*it);
			}
		}

#pragma omp for
		for (int i = 0; i < aliveCellNeighbors.size(); i++) {
			auto it = aliveCellNeighbors.begin();
			std::advance(it, i);

			if (aliveCells.find(it->first) != aliveCells.end()) // is this cell alive?
				continue; // if so skip because we already updated aliveCells

			if (aliveCellNeighbors[it->first] == 3) {
				myToLive.push_back(it->first);
			}
		}

#pragma omp critical
		{
			toDie.insert(toDie.end(), myToDie.begin(), myToDie.end());
			toLive.insert(toLive.end(), myToLive.begin(), myToLive.end());
		}
	}

	for (const Cell& deadCell : toDie) {
		setDead(deadCell);
	}

	for (const Cell& liveCell : toLive) {
		setAlive(liveCell);
	}
}

void GameOfLife::setDead(Cell cell) {
	// decrement count of all neighbors
	for (const Cell& n : cell.neighbors(worldSize)) {
		aliveCellNeighbors[n]--;
	}

	// remove self from alive cells list
	aliveCells.erase(cell);
}

void GameOfLife::setAlive(Cell cell) {
	// increment count of all neighbors
	for (const Cell& n : cell.neighbors(worldSize)) {
		aliveCellNeighbors[n]++;
	}

	// add self to alive cells list
	aliveCells.insert(cell);
}

//void GameOfLife::updateMP()
//{
//	// clear aliveCells cache
//	aliveCells.clear();
//
//#pragma omp parallel
//	{
//		// private, per-thread variables
//		auto this_thread_color = getThreadColor(omp_get_thread_num());
//		std::vector<Cell> next_generation;
//
//#pragma omp for
//		for (int i = 0; i < worldSize.x * worldSize.y; ++i) {
//			auto pos = get2D(i);
//
//			int aliveCount = 0;
//
//			// check all 8 neighbors
//			for (int nX = -1; nX <= 1; ++nX) {
//				for (int nY = -1; nY <= 1; ++nY) {
//					// skip the current cell
//					if (nX == 0 && nY == 0) continue;
//
//					// wrap around to other side if neighbor would be outside world
//					int newX = (nX + pos.x + worldSize.x) % worldSize.x;
//					int newY = (nY + pos.y + worldSize.y) % worldSize.y;
//
//					aliveCount += getCell(newX, newY);
//				}
//			}
//
//			// Evaluate game rules on current cell
//			bool dies = aliveCount == 2 || aliveCount == 3;
//			bool lives = aliveCount == 3;
//			worldBuffer[i] = world[i] ? dies : lives;
//
//			// if the cell's alive push it into the vector
//			if (worldBuffer[i])
//				next_generation.emplace_back(Cell{ pos, this_thread_color });
//		}
//
//#pragma omp critical
//		//aliveCells.insert(aliveCells.end(), next_generation.begin(), next_generation.end());
//	}
//
//	// apply updates
//	world.swap(worldBuffer);
//}

void GameOfLife::setCell(int x, int y, bool alive)
{
	// constrain x and y
	x = std::max(std::min(x, (int) worldSize.x - 1), 0);
	y = std::max(std::min(y, (int) worldSize.y - 1), 0);
	getCell(x, y) = alive;
	//aliveCells.push_back(Cell{ {x, y}, sf::Color::White });
}