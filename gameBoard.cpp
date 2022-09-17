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
	if (gamePieceBase->dummyValue > 0) {
		tile dummyBase;
		//dummyBase.setCurrentPosition(10, 10, colorMap[gamePieceBase->dummyValue]);
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
bool gameBoard::setRandomTile(unsigned int row, unsigned int col, unsigned int val)
{
	if (row == 99 || col == 99)
		return false;
	gameArray[row][col].value = val;
	gameArray[row][col].curPos.x = 200 + static_cast<float>(row) * 100;
	gameArray[row][col].curPos.y = 200 + static_cast<float>(col) * 100;
	gameArray[row][col].setCurrentPosition(row, col, colorMap[val]);
	return true;
}

bool gameBoard::renderGameBoard(sf::RenderWindow *window)
{
	bool activeAnimation = false;
	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			sf::RectangleShape tileBG(sf::Vector2f(100.f, 100.f));
			if (row == 3)
				tileBG.setSize(sf::Vector2f(95.f, 100.f));
			if (col == 3)
				tileBG.setSize(sf::Vector2f(100.f, 95.f));
			tileBG.setPosition(200 + row * 100, 200 + col * 100);
			tileBG.setOutlineThickness(5.0);
			tileBG.setOutlineColor(sf::Color(213, 198, 179, 255));
			sf::Color numberColor(223, 208, 189, 255);
			tileBG.setFillColor(numberColor);
			window->draw(tileBG);

			if (gameArray[row][col].value != 0 && gameArray[row][col].isActive == false)
			{
				drawGamePiece(&gameArray[row][col], &*window);
			}
		}
	}
	std::queue<tile> newStack;

	while (!animationQueue.empty())
	{
		activeAnimation = true;
		tile currTile = animationQueue.front();
		animationQueue.pop();
		currTile.setTileFramePos();
		drawGamePiece(&currTile, &*window);
		if (currTile.isActive)
		{
			newStack.push(currTile);
		}
		else
		{
			gameArray[currTile.origin.x][currTile.origin.y].isActive = gameArray[currTile.destinationRowCol.x][currTile.destinationRowCol.y].isActive = false;
		}
	}
	animationQueue = newStack;
	return activeAnimation;
}
unsigned int *gameBoard::returnRandomTile()
{
	int x, y, value;
	x = y = value = 99;
	unsigned int randomTile[3];
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
	randomTile[0] = x;
	randomTile[1] = y;
	randomTile[2] = value;

	return randomTile;
}
void gameBoard::initGameBoard()
{
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			gameArray[row][col].value = 0;
		}
	}
	unsigned int *newRandomTileDat = returnRandomTile();
	if (!setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2)))
	{
		std::cout << "Gameover?";
	}
}
bool gameBoard::moveRight()
{
	bool success = false;
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
					newAnimatedTile.dummyValue = gameArray[col_2 + 1][row].value;
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
					newAnimatedTile.dummyValue = thisValue;
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
					newAnimatedTile.dummyValue = thisValue;
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

	for (unsigned int row = 0; row < 4; row++)
	{
		unsigned int streak = 0;
		int memValue = 0;

		for (unsigned int col = 0; col < 4; col++)
		{

			unsigned int thisValue = gameArray[row][col].value;

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
				unsigned int col_2 = col;
				unsigned int thisValueCpy = thisValue;
				tile newAnimatedTile;
				do
				{
					col_2--;
				} while (col_2 < 4 && gameArray[row][col_2].value == 0);
				col_2++;

				if (col_2 != col)
				{
					gameArray[row][col_2].value = thisValue;
					gameArray[row][col_2].setCurrentPosition(row, col_2, colorMap[thisValue]);
					gameArray[row][col].resetTile();
					success = true;
				}

				if (thisValue == memValue && streak == 2)
				{
					for (unsigned int index = 0; index < 2; index++)
					{
						gameArray[row][col_2 - index].resetTile();
						if (index == 2 - 1)
						{
							newAnimatedTile.dummyValue = thisValue;
							gameArray[row][col_2 - index].value = thisValueCpy = thisValue * 2;
							gameArray[row][col_2 - index].setCurrentPosition(row, col_2 - index, colorMap[thisValue * 2]);
							col_2 = col_2 - index;
						}
					}
				}
				if (col_2 != col)
				{
					gameArray[row][col].isActive = gameArray[row][col_2].isActive = true;
					newAnimatedTile.setCurrentPosition(row, col, colorMap[thisValue]);
					newAnimatedTile.setDestinationPosition(row, col_2, colorMap[thisValue], colorMap[thisValueCpy]);
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
	unsigned int *newRandomTileDat;
	
	switch (keycode)
	{
	case 73:
		success = moveUp();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2)))
		{
			std::cout << "Gameover?";
		}
		break;
	case 74:
		success = moveDown();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2)))
		{
			std::cout << "Gameover?";
		}
		break;
	case 71:
		success = moveLeft();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2)))
		{
			std::cout << "Gameover?";
		}
		break;
	case 72:
		success = moveRight();
		newRandomTileDat = returnRandomTile();
		if (success && !setRandomTile(*(newRandomTileDat), *(newRandomTileDat + 1), *(newRandomTileDat + 2)))
		{
			std::cout << "Gameover?";
		}
		break;
	}
	return success;
}