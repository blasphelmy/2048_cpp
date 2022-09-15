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

class gameBoard {
private:
	unsigned int gameArray[4][4];
public:
	void drawGrid(sf::RenderWindow* window) {
		sf::Font font;
		if (!font.loadFromFile("OSReg.ttf"))
		{
			std::cout << "Failed to load font" << std::endl;
		}
		int row, col;
		for (row = 0; row < 4; row++) {
			for (col = 0; col < 4; col++) {
				sf::Text number;
				number.setFont(font);
				number.setString(std::to_string(gameArray[row][col]));
				number.setCharacterSize(24);
				number.setFillColor(sf::Color::Cyan);
				number.setPosition(235 + row * 100, 230 + col * 100);

				window->draw(number);

				sf::Vertex line_col[] =
				{
					sf::Vertex(sf::Vector2f(100 + ((row + 1) * 100), 200 + ((col + 1) * 100))),
					sf::Vertex(sf::Vector2f(100 + ((row + 1) * 100),  100 + ((col + 1) * 100)))
				};

				sf::Vertex line_row[] =
				{
					sf::Vertex(sf::Vector2f(200 + ((row + 1) * 100), 100 + ((col + 1) * 100))),
					sf::Vertex(sf::Vector2f(100 + ((row + 1) * 100), 100 + ((col + 1) * 100)))
				};

				if (col == 0 && row == 0) {
					sf::Vertex line_row_end[] =
					{
						sf::Vertex(sf::Vector2f(200, 600)),
						sf::Vertex(sf::Vector2f(600, 600))
					};
					sf::Vertex line_col_end[] =
					{
						sf::Vertex(sf::Vector2f(600, 200)),
						sf::Vertex(sf::Vector2f(600, 600))
					};
					window->draw(line_col_end, 2, sf::Lines);
					window->draw(line_row_end, 2, sf::Lines);
				}

				window->draw(line_col, 2, sf::Lines);
				window->draw(line_row, 2, sf::Lines);
			}
		}
	}
	unsigned int* returnRandomTile() {
		unsigned int x, y, value;
		x = y = value = NULL;
		unsigned int randomTile[3];
		bool emptySpots = false;

		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int col = 0; col < 4; col++) {
				if (gameArray[row][col] == 0) {
					emptySpots = true;
					break;
				}
			}
		}
		srand((unsigned)time(NULL));
		do {
			x = rand() % 4;
			y = rand() % 4;
		} while (gameArray[x][y] != 0 && emptySpots == true);

		value = 2;

		if (rand() % 100 > 55) {
			value = 4;
		}
		randomTile[0] = x;
		randomTile[1] = y;
		randomTile[2] = value;

		return randomTile;
	}
	bool setRandomTile(unsigned int row, unsigned int col, unsigned int val) {
		if (row == NULL || col == NULL) return false;
		gameArray[row][col] = val;
		return true;
	}
	void initGameBoard() {
		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int col = 0; col < 4; col++) {
				gameArray[row][col] = 0;
			}
		}
		unsigned int* newRandomTileDat = returnRandomTile();
		if (setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
			std::cout << "Gameover?";
		}
	}
	void doSomething() {
		unsigned int* newRandomTileDat = returnRandomTile();
		gameArray[*(newRandomTileDat)][*(newRandomTileDat + 1)] = *(newRandomTileDat + 2);
		if (setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
			std::cout << "Gameover?";
		}
	}
	bool moveLeft() {
		bool success = false;
		unsigned int moveTotal = 0;

		for (unsigned int row = 0; row < 4; row++) {
			unsigned int streak = 0;
		}

		return success;
	}
	bool actuateBoard(unsigned int keycode) {
		bool success = false;
		switch (keycode) {
		case 71:
			success = moveLeft();
			break;
		}
	}
	gameBoard() {
		initGameBoard();
	}
};

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
			/*std::cout << "Keypressed!";
			newGameBoard.doSomething();*/
			//left = 71 up = 73 right = 72 down = 74
			std::cout << event.key.code << std::endl;
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
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		window.clear(sf::Color::Black);
		newGameBoard.drawGrid(&window);
		mainControls(&window);
		window.display();
	}
	return 0;
}
