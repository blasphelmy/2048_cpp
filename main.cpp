#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <stdio.h>
#include <string>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "gameBoard.h"

gameBoard newGameBoard;

void mainControls(sf::RenderWindow* window) {
	sf::Event event;

	while (window->pollEvent(event))
	{
		// check the type of the event...
		switch (event.type)
		{
		// window closed
		case sf::Event::Closed:
			window->close();
			break;

		// key pressed
		case sf::Event::KeyPressed:
			//left = 71 up = 73 right = 72 down = 74
			newGameBoard.actuateBoard(event.key.code);
			break;

		// we don't process other types of events
		default:
			break;
		}
	}
}
int main(int argc, char** argv) {
	sf::RenderWindow window;
	window.create(sf::VideoMode(768, 768), "2048");
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(144);

	while (window.isOpen()) {
		window.clear(sf::Color(211, 193, 172, 255));
		newGameBoard.renderGameBoard(&window);
		mainControls(&window);
		window.display();
	}
	return 0;
}
