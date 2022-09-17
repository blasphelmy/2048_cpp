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

void tile::setCurrentPosition(unsigned int x, unsigned int y) {
	origin.x = x;
	origin.y = y;
	curPos.x = 200 + x * 100;
	curPos.y = 200 + y * 100;
}
void tile::setDestinationPosition(unsigned int x, unsigned int y) {
	destinationRowCol.x = x;
	destinationRowCol.y = y;
	isActive = true;
	desPos.x = 200 + x * 100;
	desPos.y = 200 + y * 100;
	velocity = (desPos - curPos);
	velocity.x /= velocityFactor;
	velocity.y /= velocityFactor;
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
}