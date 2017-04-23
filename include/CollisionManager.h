#pragma once

#include <vector>
#include "Collidable.h"
#include "SFML\Graphics\Drawable.hpp"
#include "SFML\Graphics\RenderTarget.hpp"

class CollisionManager : public sf::Drawable {
public:
	CollisionManager();
	void addCollidable(Collidable* collidable);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool raycast(const sf::Vector2f& from, const sf::Vector2f& to) const;

private:
	bool rectCollision(sf::FloatRect A, sf::FloatRect B);
	bool lineRectCollision(sf::FloatRect A, const sf::Vector2f& lineStart, const sf::Vector2f& lineEnd) const;
	std::vector<Collidable*> m_collidables;
};
