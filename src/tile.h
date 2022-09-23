#pragma once
#include <stdio.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#ifndef TILE_H
#define TILE_H

class tile {
public:
	unsigned int velocityFactor = 15;
	tile() {
		isActive = false;
		value = 0;
		dummyValue = 0;
	}
	bool isActive;
	unsigned int value;
	unsigned int dummyValue;
	sf::Vector3f color;
	sf::Vector2i origin;
	sf::Vector2i destinationRowCol;
	sf::Vector2f velocity;
	sf::Vector3f colorChangeVelocity;
	sf::Vector2f curPos;
	sf::Vector2f desPos;
	sf::Color getColor(std::map<unsigned int, sf::Color>* colorMap);
	sf::Vector2f getCurrentPos();
	void setCurrentPosition(unsigned int x, unsigned int y, sf::Color newColor);
	void setDestinationPosition(unsigned int x, unsigned int y, sf::Color oldColor, sf::Color newColor);
	void resetTile();
	void setTileFramePos();
};

#endif