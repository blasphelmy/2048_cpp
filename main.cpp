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

const int fps = 120;

gameBoard newGameBoard;
sf::Font font;

void mainControls(sf::RenderWindow* window, bool isActive) {
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
			if(!isActive) newGameBoard.actuateBoard(event.key.code);
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
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(fps);
	if (!font.loadFromFile("OSReg.ttf"))
	{
		std::cout << "Failed to load font" << std::endl;
	}
	newGameBoard.font = font;
	while (window.isOpen()) {
		//std::cout << "Loop" << std::endl;
		window.clear(sf::Color(211, 193, 172, 255));
		bool isActive = newGameBoard.renderGameBoard(&window);
		mainControls(&window, isActive);
		window.display();
	}
	return 0;
}
