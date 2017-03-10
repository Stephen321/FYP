#pragma once

#include "Screen.h"

class GameOverScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
};