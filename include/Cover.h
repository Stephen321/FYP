#pragma once

#include "SFML\Graphics.hpp"

struct Cover {
	Cover() : occupied(false) {};
	sf::Vector2f position;
	bool occupied;
};