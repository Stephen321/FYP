#pragma once

#include "Screen.h"

class MenuScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
};