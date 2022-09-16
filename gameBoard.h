#pragma once
#include <stdio.h>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stack>
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class tile {
private:
public:
	bool isActive = false;
	unsigned int value = 0;
	sf::Vector2i origin;
	sf::Vector2i destinationRowCol;
	sf::Vector2f velocity;
	sf::Vector2f curPos;
	sf::Vector2f desPos;
	sf::Vector2f lastPos;
	
	sf::Vector2f getCurrentPos() {
		sf::Vector2f thisCurPos;
		
		thisCurPos = curPos;
		
		return thisCurPos;
	}
	void setCurrentPosition(unsigned int x, unsigned int y) {
		origin.x = x;
		origin.y = y;
		curPos.x = 200 + x * 100;
		curPos.y = 200 + y * 100;
	}
	void setDestinationPosition(unsigned int x, unsigned int y) {
		destinationRowCol.x = x;
		destinationRowCol.y = y;
		isActive = true;
		desPos.x = 200 + x * 100;
		desPos.y = 200 + y * 100;
		velocity = (desPos - curPos);
		velocity.x /= 15;
		velocity.y /= 15;
	}
	void resetTile() {
		isActive = false;
		velocity.y = velocity.x = 0.f;
		curPos = sf::Vector2f();
		desPos = sf::Vector2f();
		lastPos = sf::Vector2f();
		value = 0;
	}
	void setTileFramePos() {
		if (velocity.x > 0 && velocity.y == 0) {
			curPos = curPos + velocity;
			if (curPos.x >= desPos.x) {
				isActive = false;
				curPos.x = desPos.x;
			}
		}
		else if (velocity.x < 0 && velocity.y == 0) {
			curPos = curPos + velocity;
			if (curPos.x <= desPos.x) {
				isActive = false;
				curPos.x = desPos.x;
			}
		}
		else if (velocity.x == 0 && velocity.y > 0) {
			curPos = curPos + velocity;
			if (curPos.y >= desPos.y) {
				isActive = false;
				curPos.y = desPos.y;
			}
		}
		else if (velocity.x == 0 && velocity.y < 0) {
			curPos = curPos + velocity;
			if (curPos.y <= desPos.y) {
				isActive = false;
				curPos.y = desPos.y;
			}
		}
	}
};

class gameBoard {
private:
	tile gameArray[4][4];
	std::stack <tile> animationQueue;
	std::stack <tile> animationQueueSwap;
	std::map<unsigned int, sf::Color> colorMap;
	void initializeColorMap() {
		colorMap[2] =     sf::Color(238, 228, 218, 255);
		colorMap[4] =     sf::Color(238, 225, 201, 255);
		colorMap[8] =     sf::Color(243, 178, 122, 255);
		colorMap[16] =    sf::Color(246, 150, 100, 255);
		colorMap[32] =    sf::Color(51, 173, 86, 255);
		colorMap[64] =    sf::Color(62, 193, 62, 255);
		colorMap[128] =   sf::Color(50, 168, 164, 255);
		colorMap[256] =   sf::Color(50, 146, 168, 255);
		colorMap[512] =   sf::Color(186, 47, 47, 255);
		colorMap[1024] =  sf::Color(219, 35, 112, 255);
		colorMap[2048] =  sf::Color(184, 37, 184, 255);
		colorMap[4096] =  sf::Color(255, 77, 187, 255);
		colorMap[8192] =  sf::Color(214, 0, 107, 255);
		colorMap[16384] = sf::Color(204, 51, 255, 255);
		colorMap[32768] = sf::Color(82, 82, 82, 255);
	}
public:
	sf::Font font;
	sf::Color getNumberColor(unsigned int value) {
		if (colorMap.count(value)) {
			return colorMap[value];
		}
		return colorMap[32768];
	}
	void drawGamePiece(tile * gamePieceBase, sf::RenderWindow * window) {
		sf::RectangleShape gamePiece(sf::Vector2f(95.f, 95.f));
		sf::Text number{ std::to_string(gamePieceBase->value), font };
		sf::Color numberColor = getNumberColor(gamePieceBase->value);

		gamePiece.setFillColor(numberColor);
		//std::cout << gamePieceBase.getCurrentPos().x << "  " << gamePieceBase.getCurrentPos().y << std::endl;
		gamePiece.setPosition(gamePieceBase->getCurrentPos().x, gamePieceBase->getCurrentPos().y);

		number.setCharacterSize(24);
		if (gamePieceBase->value <= 4) {
			number.setFillColor(sf::Color::Black);
		}
		else {
			number.setFillColor(sf::Color::White);
		}
		sf::Vector2f bounds = gamePiece.getPosition();

		sf::FloatRect innerBounds = number.getLocalBounds();
		number.setPosition(bounds.x + ((90 - innerBounds.width) / 2), bounds.y + ((85 - innerBounds.height) / 2));
		window->draw(gamePiece);
		window->draw(number);
	}
	bool renderGameBoard(sf::RenderWindow * window) {
		bool activeAnimation = false;
		int row, col;
		for (row = 0; row < 4; row++) {
			for (col = 0; col < 4; col++) {
				sf::RectangleShape tileBG(sf::Vector2f(100.f, 100.f));
				if (row == 3) tileBG.setSize(sf::Vector2f(95.f, 100.f));
				if (col == 3) tileBG.setSize(sf::Vector2f(100.f, 95.f));
				tileBG.setPosition(200 + row * 100, 200 + col * 100);
				tileBG.setOutlineThickness(5.0);
				tileBG.setOutlineColor(sf::Color(213, 198, 179, 255));
				sf::Color numberColor(223, 208, 189, 255);
				tileBG.setFillColor(numberColor);
				window->draw(tileBG);
			}
		}

		for (row = 0; row < 4; row++) {
			for (col = 0; col < 4; col++) {
				sf::Color numberColor(223, 208, 189, 255);

				if (gameArray[row][col].value != 0 && gameArray[row][col].isActive == false) {
					drawGamePiece(&gameArray[row][col], &*window);
				}
			}
			activeAnimation = false;
		}
		if (!animationQueue.empty()) {
			std::stack<tile> newStack;

			while (!animationQueue.empty()) {
				tile currTile = animationQueue.top();
				animationQueue.pop();
				currTile.setTileFramePos();
				drawGamePiece(&currTile, &*window);
				if (currTile.isActive) {
					newStack.push(currTile);
				}
				else {
					gameArray[currTile.origin.x][currTile.origin.y].isActive = gameArray[currTile.destinationRowCol.x][currTile.destinationRowCol.y].isActive = false;
				}
			}
			animationQueue = newStack;

			activeAnimation = true;
		}
		return activeAnimation;
	}
	unsigned int * returnRandomTile() {
		int x, y, value;
		x = y = value = 99;
		unsigned int randomTile[3];
		bool emptySpots = false;

		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int col = 0; col < 4; col++) {
				if (gameArray[row][col].value == 0) {
					emptySpots = true;
					break;
				}
			}
		}
		srand((unsigned)time(NULL));
		do {
			if (emptySpots == true){
				x = rand() % 4;
				y = rand() % 4;
			}
		} while (gameArray[x][y].value != 0 && emptySpots == true);


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
		if (row == 99 || col == 99) return false;
		gameArray[row][col].value = val;
		gameArray[row][col].curPos.x = 200 + row * 100;
		gameArray[row][col].curPos.y = 200 + col * 100;
		return true;
	}
	void initGameBoard() {
		for (unsigned int row = 0; row < 4; row++) {
			for (unsigned int col = 0; col < 4; col++) {
				gameArray[row][col].value = 0;
				gameArray[row][col].lastPos.x  = 0;
				gameArray[row][col].lastPos.y = 0;
			}
		}
		unsigned int* newRandomTileDat = returnRandomTile();
		if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
			std::cout << "Gameover?";
		}
	}
	bool moveRight() {
		bool success = false;
		for (int row = 0; row < 4; row++) {
			int streak = 0;
			int memValue = 0;
			for (int col = 3; col >= 0; col--) {
				int thisValue = gameArray[col][row].value;

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (memValue == thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					int col_2 = col;
					while (col_2 + 1 < 4 && gameArray[col_2 + 1][row].value == 0) {
						col_2++;
					}
					if (col_2 != col) {
						gameArray[col][row].resetTile();
						gameArray[col_2][row].value = thisValue;
						gameArray[col_2][row].setCurrentPosition(col_2, row);
						success = true;
					}
					if (thisValue == memValue && streak == 2 && col_2 + 1 < 4) {
						gameArray[col_2 + 1][row].value = thisValue * 2;
						gameArray[col_2][row].value = 0;
						gameArray[col_2][row].resetTile();
						gameArray[col_2 + 1][row].value = thisValue * 2;
						gameArray[col_2 + 1][row].setCurrentPosition(col_2 + 1, row);
						col_2++;
					}
					if (col_2 != col) {
						gameArray[col][row].isActive = gameArray[col_2][row].isActive = true;
						tile newAnimatedTile;
						newAnimatedTile.setCurrentPosition(col, row);
						newAnimatedTile.setDestinationPosition(col_2, row);
						newAnimatedTile.value = gameArray[col_2][row].value;
						animationQueue.push(newAnimatedTile);
					}
				}

			}
		}
		return success;
	}
	bool moveLeft() {
		bool success = false;
		
		for (int row = 3; row >= 0; row--) {
			int streak = 0;
			unsigned int memValue = 0;
			for (int col = 0; col < 4; col++) {
				unsigned int thisValue = gameArray[col][row].value;

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (thisValue != 0 && memValue == thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					int col_2 = col;
					while (col_2 > 0 && gameArray[col_2 - 1][row].value == 0) {
						col_2--;
					}

					if (col_2 != col) {
						gameArray[col][row].resetTile();
						gameArray[col_2][row].value = thisValue;
						gameArray[col_2][row].setCurrentPosition(col_2, row);
						success = true;
					}
					if (thisValue == memValue && streak == 2 && col_2 - 1 >= 0) {
						gameArray[col_2][row].resetTile();
						gameArray[col_2 - 1][row].value = thisValue * 2;
						gameArray[col_2 - 1][row].setCurrentPosition(col_2 - 1, row);
						col_2--;
					}
					if (col_2 != col) {
						gameArray[col][row].isActive = gameArray[col_2][row].isActive = true;
						tile newAnimatedTile;
						newAnimatedTile.setCurrentPosition(col, row);
						newAnimatedTile.setDestinationPosition(col_2, row);
						newAnimatedTile.value = gameArray[col_2][row].value;
						animationQueue.push(newAnimatedTile);
					}
				}
			}
		}

		return success;
	}
	bool moveDown(){
		bool success = false;
		unsigned int moveTotal = 0;

		for (int col = 0; col < 4; col++) {
			int streak = 0;
			int memValue = 0;

			for (int row = 3; row >= 0; row--) {
				int thisValue = gameArray[col][row].value;

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (memValue == thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					int row_2 = row;
					while (row_2 + 1 < 4 && gameArray[col][row_2 + 1].value == 0) {
						row_2++;
					}
					if (row_2 != row) {
						gameArray[col][row].resetTile();
						gameArray[col][row_2].value = thisValue;
						gameArray[col][row_2].setCurrentPosition(col, row_2);
						success = true;
					}

					if (thisValue == memValue && streak == 2 && row_2 + 1 < 4) {
						gameArray[col][row_2 + 1].value = thisValue * 2;
						gameArray[col][row_2].value = 0;
						gameArray[col][row_2].resetTile();
						gameArray[col][row_2 + 1].value = thisValue * 2;
						gameArray[col][row_2 + 1].setCurrentPosition(col, row_2 + 1);
						row_2++;
					}
					if (row_2 != row) {
						gameArray[col][row].isActive = gameArray[col][row_2].isActive = true;
						tile newAnimatedTile;
						newAnimatedTile.setCurrentPosition(col, row);
						newAnimatedTile.setDestinationPosition(col, row_2);
						newAnimatedTile.value = gameArray[col][row_2].value;
						animationQueue.push(newAnimatedTile);
					}
				}
			}
		}
		return success;
	}
	bool moveUp() {
		bool success = false;
		unsigned int moveTotal = 0;

		for (unsigned int row = 0; row < 4; row++) {
			unsigned int streak = 0;
			int memValue = 0;

			for (unsigned int col = 0; col < 4; col++) {
				
				unsigned int thisValue = gameArray[row][col].value;

				if (thisValue != 0 && thisValue != memValue) {
					memValue = thisValue;
					streak = 1;
				}
				else if (memValue == thisValue) {
					streak++;
				}

				if (thisValue != 0) {
					unsigned int col_2 = col;

					do {
						col_2--;
					} while (col_2 < 4 && gameArray[row][col_2].value == 0);
					col_2++;

					if (col_2 != col) {
						gameArray[row][col_2].value = thisValue;
						gameArray[row][col_2].setCurrentPosition(row, col_2);
						gameArray[row][col].resetTile();
						success = true;
					}

					if (thisValue == memValue && streak == 2) {
						for (unsigned int index = 0; index < 2; index++) {
							gameArray[row][col_2 - index].resetTile();
							if (index == 2 - 1) {
								// this.actuateTile(row, col_2, row, col_2 - index, thisValue * this.requiredStreak);
								gameArray[row][col_2 - index].value = thisValue * 2;
								gameArray[row][col_2 - index].setCurrentPosition(row, col_2 - index);
								col_2 = col_2 - index;
							}
						}

					}
					if (col_2 != col) {
						gameArray[row][col ].isActive = gameArray[row][col_2].isActive = true;
						tile newAnimatedTile;
						newAnimatedTile.setCurrentPosition(row, col);
						newAnimatedTile.setDestinationPosition(row, col_2);
						newAnimatedTile.value = gameArray[row][col_2].value;
						animationQueue.push(newAnimatedTile);
					}
				}

			}
		}
		return success;
	}
	bool actuateBoard(unsigned int keycode) {
		bool success = false;
		unsigned int * newRandomTileDat;
		switch (keycode) {
		case 73:
			success = moveUp();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		case 74 :
			success = moveDown();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		case 71:
			success = moveLeft();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		case 72:
			success = moveRight();
			newRandomTileDat = returnRandomTile();
			if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2))) {
				std::cout << "Gameover?";
			}
			break;
		}
		return success;
	}
	gameBoard() {
		initGameBoard();
		initializeColorMap();
	}
};

#endif