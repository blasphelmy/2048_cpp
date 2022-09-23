#include "tile.h"

#include <stdio.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::Vector2f tile::getCurrentPos() {
	sf::Vector2f thisCurPos;
	thisCurPos = curPos;

	return thisCurPos;
}
sf::Color tile::getColor(std::map<unsigned int, sf::Color> * colorMap) {
	return sf::Color(color.x, color.y, color.z);
}
void tile::setCurrentPosition(unsigned int x, unsigned int y, sf::Color newColor) {
	origin.x = x;
	origin.y = y;
	curPos.x = 200 + x * 100;
	curPos.y = 200 + y * 100;
	color = sf::Vector3f(static_cast<float>(newColor.r), static_cast<float>(newColor.g), static_cast<float>(newColor.b));
}
void tile::setDestinationPosition(unsigned int x, unsigned int y, sf::Color oldColor, sf::Color newColor) {
	destinationRowCol.x = x;
	destinationRowCol.y = y;
	isActive = true;
	desPos.x = 200 + x * 100;
	desPos.y = 200 + y * 100;
	velocity = (desPos - curPos);
	velocity.x /= velocityFactor;
	velocity.y /= velocityFactor;

	color = sf::Vector3f(static_cast<float>(oldColor.r), static_cast<float>(oldColor.g), static_cast<float>(oldColor.b));
	sf::Vector3f destinationColor(static_cast<float>(newColor.r), static_cast<float>(newColor.g), static_cast<float>(newColor.b));
	colorChangeVelocity = destinationColor - color;
	colorChangeVelocity.x = colorChangeVelocity.x / velocityFactor;
	colorChangeVelocity.y = colorChangeVelocity.y / velocityFactor;
	colorChangeVelocity.z = colorChangeVelocity.z / velocityFactor;
}
void tile::resetTile() {
	isActive = false;
	velocity.y = velocity.x = 0.f;
	curPos = sf::Vector2f();
	desPos = sf::Vector2f();
	value = 0;
	dummyValue = 0;
}

void tile::setTileFramePos() {
	curPos = curPos + velocity;
	if (velocity.x > 0 && velocity.y == 0) {
		if (curPos.x >= desPos.x) {
			isActive = false;
			curPos.x = desPos.x;
		}
	}
	else if (velocity.x < 0 && velocity.y == 0) {
		if (curPos.x <= desPos.x) {
			isActive = false;
			curPos.x = desPos.x;
		}
	}
	else if (velocity.x == 0 && velocity.y > 0) {
		if (curPos.y >= desPos.y) {
			isActive = false;
			curPos.y = desPos.y;
		}
	}
	else if (velocity.x == 0 && velocity.y < 0) {
		if (curPos.y <= desPos.y) {
			isActive = false;
			curPos.y = desPos.y;
		}
	}
	color = color + colorChangeVelocity;
}