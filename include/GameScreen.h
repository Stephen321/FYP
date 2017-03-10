#pragma once

#include "Screen.h"
#include "Player.h"
#include "GameLoader.h"

class GameScreen : public Screen
{
public:
	int run(sf::RenderWindow &window) override;
private: 
	sf::FloatRect getRectFromView(const sf::View& view);
	void drawGameObject(sf::RenderTarget& target, std::shared_ptr<GameObject>& gameObject, const sf::FloatRect& viewBounds);
	void checkForCollisions(std::vector<std::shared_ptr<GameObject>>& v, std::shared_ptr<GameObject>& collider);
};