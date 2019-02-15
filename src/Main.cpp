#include "GameOfLife.h"
#include "WorldRenderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(256, 256), "My window");
	// scale the image up 2x size
	window.setSize(sf::Vector2u(512, 512));

	// Create the game
	GameOfLife game;

	// Create a world renderer
	WorldRenderer worldRenderer;

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
				{
					game.update();

					/*std::cout << "the left button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;*/
				}
			}
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// update the game
		//game.update();

		// render the game
		worldRenderer.render(window, game);

		// end the current frame
		window.display();
	}

	return 0;
}
