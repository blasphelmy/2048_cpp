#pragma once
#include "tile.h"

#include <stdio.h>
#include <iostream>
#include <queue>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class gameBoard {
private:
	unsigned int score;
	tile gameArray[4][4];
	std::queue <tile> animationQueue;
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
	sf::Color getNumberColor(unsigned int value);
	std::vector<std::vector<bool>> generate2dArray(unsigned int row, unsigned int col);
	void drawGamePiece(tile* gamePieceBase, sf::RenderWindow* window);
	bool renderGameBoard(sf::RenderWindow* window);
	sf::Vector3i returnRandomTile();
	bool setRandomTile(sf::Vector3i data);
	void initGameBoard();
	bool moveRight();
	bool moveLeft();
	bool moveDown();
	bool moveUp();
	bool actuateBoard(int keycode);
	gameBoard() {
		initializeColorMap();
		initGameBoard();
	}
};

#endif