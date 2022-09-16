#pragma once
#include <stdio.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#ifndef TILE_H
#define TILE_H

class tile {
public:
	tile() {
		isActive = false;
		value = 0;
	}
	bool isActive;
	unsigned int value;
	sf::Vector2i origin;
	sf::Vector2i destinationRowCol;
	sf::Vector2f velocity;
	sf::Vector2f curPos;
	sf::Vector2f desPos;
	sf::Vector2f getCurrentPos();
	void setCurrentPosition(unsigned int x, unsigned int y);
	void setDestinationPosition(unsigned int x, unsigned int y);
	void resetTile();
	void setTileFramePos();
};

#endif