#include "gameBoard.h"

#include <stdio.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::Color gameBoard::getNumberColor(unsigned int value)
{
	if (colorMap.count(value))
	{
		return colorMap[value];
	}
	return colorMap[32768];
}

void gameBoard::drawGamePiece(tile *gamePieceBase, sf::RenderWindow *window)
{
	if (gamePieceBase->dummyValue > 0)
	{
		tile dummyBase;
		// dummyBase.setCurrentPosition(10, 10, colorMap[gamePieceBase->dummyValue]);
		dummyBase.value = gamePieceBase->value;
		dummyBase.curPos = gamePieceBase->desPos;
		dummyBase.color = gamePieceBase->color;
		drawGamePiece(&dummyBase, &*window);
	}

	sf::RectangleShape gamePiece(sf::Vector2f(95.f, 95.f));
	sf::Text number{std::to_string(gamePieceBase->value), font};
	sf::Color numberColor((gamePieceBase->color.x), (gamePieceBase->color.y), (gamePieceBase->color.z));
	gamePiece.setFillColor(numberColor);
	gamePiece.setPosition(gamePieceBase->getCurrentPos().x, gamePieceBase->getCurrentPos().y);

	number.setCharacterSize(24);
	if (gamePieceBase->value <= 4)
	{
		number.setFillColor(sf::Color::Black);
	}
	else
	{
		number.setFillColor(sf::Color::White);
	}
	sf::Vector2f bounds = gamePiece.getPosition();

	sf::FloatRect innerBounds = number.getLocalBounds();
	number.setPosition(bounds.x + ((90 - innerBounds.width) / 2), bounds.y + ((85 - innerBounds.height) / 2));
	window->draw(gamePiece);
	window->draw(number);
}
bool gameBoard::setRandomTile(sf::Vector3i data)
{
	if (data.x == 99 || data.y == 99) return false;

	gameArray[data.x][data.y].value = data.z;
	gameArray[data.x][data.y].curPos.x = 200 + static_cast<float>(data.x) * 100;
	gameArray[data.x][data.y].curPos.y = 200 + static_cast<float>(data.y) * 100;
	gameArray[data.x][data.y].setCurrentPosition(data.x, data.y, colorMap[data.z]);
	return true;
}

bool gameBoard::renderGameBoard(sf::RenderWindow *window)
{
	bool activeAnimation = false;
	int row, col;

	sf::Text scoreElement = sf::Text("Score : " + std::to_string(score), font);

	window->draw(scoreElement);

	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			sf::RectangleShape tileBG(sf::Vector2f(100.f, 100.f));
			if (row == 3)
				tileBG.setSize(sf::Vector2f(95.f, 100.f));
			if (col == 3)
				tileBG.setSize(sf::Vector2f(100.f, 95.f));
			if (row == 3 && col == 3)
			{
				tileBG.setSize(sf::Vector2f(95.f, 95.f));
			}
			tileBG.setPosition(200 + row * 100, 200 + col * 100);
			tileBG.setOutlineThickness(5.0);
			tileBG.setOutlineColor(sf::Color(203, 188, 169, 255));
			sf::Color numberColor(223, 208, 189, 255);
			tileBG.setFillColor(numberColor);
			window->draw(tileBG);

			if (gameArray[row][col].value != 0 && gameArray[row][col].isActive == false)
			{
				drawGamePiece(&gameArray[row][col], &*window);
			}
		}
	}
	std::queue<tile> newQueue;

	while (!animationQueue.empty())
	{
		activeAnimation = true;
		tile currTile = animationQueue.front();
		animationQueue.pop();
		currTile.setTileFramePos();
		drawGamePiece(&currTile, &*window);
		if (currTile.isActive)
		{
			newQueue.push(currTile);
		}
		else
		{
			gameArray[currTile.origin.x][currTile.origin.y].isActive = gameArray[currTile.destinationRowCol.x][currTile.destinationRowCol.y].isActive = false;
		}
	}
	animationQueue = newQueue;
	return activeAnimation;
}
sf::Vector3i gameBoard::returnRandomTile()
{
	int x, y, value;
	x = y = value = 99;
	sf::Vector3i randomTile;
	bool emptySpots = false;

	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			if (gameArray[row][col].value == 0)
			{
				emptySpots = true;
				break;
			}
		}
	}
	srand((unsigned)time(NULL));
	do
	{
		if (emptySpots == true)
		{
			x = rand() % 4;
			y = rand() % 4;
		}
	} while (gameArray[x][y].value != 0 && emptySpots == true);

	value = 2;

	if (rand() % 100 > 55)
	{
		value = 4;
	}
	randomTile.x = x;
	randomTile.y = y;
	randomTile.z = value;

	return randomTile;
}
void gameBoard::initGameBoard()
{
	score = 0;
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			gameArray[row][col].value = 0;
		}
	}
	sf::Vector3i newRandomTileDat = returnRandomTile();
	if (!setRandomTile(newRandomTileDat))
	{
		std::cout << "Gameover?";
	}
}

std::vector<std::vector<bool>> gameBoard::generate2dArray(unsigned int row, unsigned int col) {
	std::vector<std::vector<bool>> memoryMap(row, std::vector<bool>(col, 0));

	for (unsigned int row = 0; row < 4; row++) {
		for (unsigned int col = 0; col < 4; col++) {
			memoryMap[col][row] = true;
			if (gameArray[col][row].value > 0) {
				memoryMap[col][row] = false;
			}
		}
	}

	return memoryMap;
}

bool gameBoard::moveRight()
{
	bool success = false;
	std::vector<std::vector<bool>> memoryMap = generate2dArray(4, 4);

	for (int row = 0; row < 4; row++)
	{
		int streak = 0;
		int memValue = 0;
		for (int col = 3; col >= 0; col--)
		{
			int thisValue = gameArray[col][row].value;

			if (thisValue != 0 && thisValue != memValue)
			{
				memValue = thisValue;
				streak = 1;
			}
			else if (memValue == thisValue)
			{
				streak++;
			}

			if (thisValue != 0)
			{
				int col_2 = col;
				unsigned int thisValueCpy = thisValue;
				tile newAnimatedTile;
				while (col_2 + 1 < 4 && gameArray[col_2 + 1][row].value == 0)
				{
					col_2++;
				}
				if (col_2 != col)
				{
					gameArray[col][row].resetTile();
					gameArray[col_2][row].value = thisValue;
					gameArray[col_2][row].setCurrentPosition(col_2, row, colorMap[thisValue]);
					success = true;
				}
				if (thisValue == memValue && streak == 2 && col_2 + 1 < 4)
				{
					score += thisValue * 2;
					if(memoryMap[col_2 + 1][row] == false) newAnimatedTile.dummyValue = gameArray[col_2 + 1][row].value;
					gameArray[col_2 + 1][row].value = thisValueCpy = thisValue * 2;
					gameArray[col_2][row].resetTile();
					gameArray[col_2 + 1][row].setCurrentPosition(col_2 + 1, row, colorMap[thisValue * 2]);
					col_2++;
				}
				if (col_2 != col)
				{
					gameArray[col][row].isActive = gameArray[col_2][row].isActive = true;
					newAnimatedTile.setCurrentPosition(col, row, colorMap[thisValue]);
					newAnimatedTile.setDestinationPosition(col_2, row, colorMap[thisValue], colorMap[thisValueCpy]);
					newAnimatedTile.value = thisValue;
					animationQueue.push(newAnimatedTile);
				}
			}
		}
	}
	return success;
}
bool gameBoard::moveLeft()
{
	bool success = false;
	std::vector<std::vector<bool>> memoryMap = generate2dArray(4, 4);

	for (int row = 3; row >= 0; row--)
	{
		int streak = 0;
		unsigned int memValue = 0;
		for (int col = 0; col < 4; col++)
		{
			unsigned int thisValue = gameArray[col][row].value;

			if (thisValue != 0 && thisValue != memValue)
			{
				memValue = thisValue;
				streak = 1;
			}
			else if (thisValue != 0 && memValue == thisValue)
			{
				streak++;
			}

			if (thisValue != 0)
			{
				int col_2 = col;
				unsigned int thisValueCpy = thisValue;
				tile newAnimatedTile;
				while (col_2 > 0 && gameArray[col_2 - 1][row].value == 0)
				{
					col_2--;
				}

				if (col_2 != col)
				{
					gameArray[col][row].resetTile();
					gameArray[col_2][row].value = thisValue;
					gameArray[col_2][row].setCurrentPosition(col_2, row, colorMap[thisValue]);
					success = true;
				}
				if (thisValue == memValue && streak == 2 && col_2 - 1 >= 0)
				{
					score += thisValue * 2;
					if(memoryMap[col_2 - 1][row] == false) newAnimatedTile.dummyValue = thisValue;
					gameArray[col_2][row].resetTile();
					gameArray[col_2 - 1][row].value = thisValueCpy = thisValue * 2;
					gameArray[col_2 - 1][row].setCurrentPosition(col_2 - 1, row, colorMap[thisValue * 2]);
					col_2--;
				}
				if (col_2 != col)
				{
					gameArray[col][row].isActive = gameArray[col_2][row].isActive = true;
					newAnimatedTile.setCurrentPosition(col, row, colorMap[thisValue]);
					newAnimatedTile.setDestinationPosition(col_2, row, colorMap[thisValue], colorMap[thisValueCpy]);
					newAnimatedTile.value = thisValue;
					animationQueue.push(newAnimatedTile);
				}
			}
		}
	}

	return success;
}
bool gameBoard::moveDown()
{
	bool success = false;
	unsigned int moveTotal = 0;
	std::vector<std::vector<bool>> memoryMap = generate2dArray(4, 4);

	for (int col = 0; col < 4; col++)
	{
		int streak = 0;
		int memValue = 0;

		for (int row = 3; row >= 0; row--)
		{
			int thisValue = gameArray[col][row].value;

			if (thisValue != 0 && thisValue != memValue)
			{
				memValue = thisValue;
				streak = 1;
			}
			else if (memValue == thisValue)
			{
				streak++;
			}

			if (thisValue != 0)
			{
				int row_2 = row;
				unsigned int thisValueCpy = thisValue;
				tile newAnimatedTile;
				while (row_2 + 1 < 4 && gameArray[col][row_2 + 1].value == 0)
				{
					row_2++;
				}
				if (row_2 != row)
				{
					gameArray[col][row].resetTile();
					gameArray[col][row_2].value = thisValue;
					gameArray[col][row_2].setCurrentPosition(col, row_2, colorMap[thisValue]);
					success = true;
				}

				if (thisValue == memValue && streak == 2 && row_2 + 1 < 4)
				{
					score += thisValue * 2;
					if(memoryMap[col][row_2 + 1] == false) newAnimatedTile.dummyValue = thisValue;
					gameArray[col][row_2 + 1].value = thisValueCpy = thisValue * 2;
					gameArray[col][row_2].resetTile();
					gameArray[col][row_2 + 1].setCurrentPosition(col, row_2 + 1, colorMap[thisValue * 2]);
					row_2++;
				}
				if (row_2 != row)
				{
					gameArray[col][row].isActive = gameArray[col][row_2].isActive = true;
					newAnimatedTile.setCurrentPosition(col, row, colorMap[thisValue]);
					newAnimatedTile.setDestinationPosition(col, row_2, colorMap[thisValue], colorMap[thisValueCpy]);
					newAnimatedTile.value = thisValue;
					animationQueue.push(newAnimatedTile);
				}
			}
		}
	}
	return success;
}
bool gameBoard::moveUp()
{
	bool success = false;
	unsigned int moveTotal = 0;
	std::vector<std::vector<bool>> memoryMap = generate2dArray(4, 4);

	for (int col = 0; col < 4; col++)
	{
		int streak = 0;
		int memValue = 0;

		for (int row = 0; row < 4; row++)
		{
			int thisValue = gameArray[col][row].value;

			if (thisValue != 0 && thisValue != memValue)
			{
				memValue = thisValue;
				streak = 1;
			}
			else if (memValue == thisValue)
			{
				streak++;
			}

			if (thisValue != 0)
			{
				int row_2 = row;
				unsigned int thisValueCpy = thisValue;
				tile newAnimatedTile;
				while (row_2 - 1 >= 0 && gameArray[col][row_2 - 1].value == 0)
				{
					row_2--;
				}
				if (row_2 != row)
				{
					gameArray[col][row].resetTile();
					gameArray[col][row_2].value = thisValue;
					gameArray[col][row_2].setCurrentPosition(col, row_2, colorMap[thisValue]);
					success = true;
				}

				if (thisValue == memValue && streak == 2 && row_2 - 1 >= 0)
				{
					score += thisValue * 2;
					if (memoryMap[col][row_2 - 1] == false) newAnimatedTile.dummyValue = thisValue;
					gameArray[col][row_2 - 1].value = thisValueCpy = thisValue * 2;
					gameArray[col][row_2].resetTile();
					gameArray[col][row_2 - 1].setCurrentPosition(col, row_2 - 1, colorMap[thisValue * 2]);
					row_2--;
				}
				if (row_2 != row)
				{
					gameArray[col][row].isActive = gameArray[col][row_2].isActive = true;
					newAnimatedTile.setCurrentPosition(col, row, colorMap[thisValue]);
					newAnimatedTile.setDestinationPosition(col, row_2, colorMap[thisValue], colorMap[thisValueCpy]);
					newAnimatedTile.value = thisValue;
					animationQueue.push(newAnimatedTile);
				}
			}
		}
	}
	return success;
}
bool gameBoard::actuateBoard(int keycode)
{
	bool success = false;
	sf::Vector3i newRandomTileDat;

	switch (keycode)
	{
	case 73:
		success = moveUp();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(newRandomTileDat))
		{
			std::cout << "Gameover?";
		}
		break;
	case 74:
		success = moveDown();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(newRandomTileDat))
		{
			std::cout << "Gameover?";
		}
		break;
	case 71:
		success = moveLeft();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(newRandomTileDat))
		{
			std::cout << "Gameover?";
		}
		break;
	case 72:
		success = moveRight();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(newRandomTileDat))
		{
			std::cout << "Gameover?";
		}
		break;
	}
	return success;
}