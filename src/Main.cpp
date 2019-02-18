#include "GameOfLife.h"
#include "WorldRenderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

static const int WORLD_SIZE_X = 256;
static const int WORLD_SIZE_Y = 256;

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(256, 256), "Game of Life");
	// scale the image up 2x size
	window.setSize(sf::Vector2u(512, 512));

	// disable vsync and uncap framerate limit
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(0);

	// Create the game
	GameOfLife game(sf::Vector2i(WORLD_SIZE_X, WORLD_SIZE_Y));

	// Create a world renderer
	WorldRenderer worldRenderer;

	// Track if mouse button is being held down
	bool mouseHeld = false;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					mouseHeld = true;
			} else if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
					mouseHeld = false;
			}
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// handle mouse input
		if (mouseHeld) {
			auto mousePosition = sf::Mouse::getPosition(window);

			// normalize mouse pos
			int x = (mousePosition.x / 512.0f) * WORLD_SIZE_X;
			int y = (mousePosition.y / 512.0f) * WORLD_SIZE_Y;

			// set cell under cursor to alive
			game.setCell(x, y, true);
		}
		else {
			// update the game
			game.update();
		}

		// render the game
		worldRenderer.render(window, game);

		// end the current frame
		window.display();
	}

	return 0;
}
