// GameOfLife.cpp : Defines the entry point for the application.
//

#include "GameOfLife.h"
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

int main()
{
	//// new world with all dead cells
	//bool world[16][16]{ false };

	// create the window
	sf::RenderWindow window(sf::VideoMode(256, 256), "My window");
	// scale the image up 2x size
	window.setSize(sf::Vector2u(512, 512));

	// Create the game
	GameOfLife game;

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
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// update the game
		game.update();

		// render the game
		game.render(window);

		// end the current frame
		window.display();
	}

	return 0;
}
