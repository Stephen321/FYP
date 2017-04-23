#pragma once

#include "ObjectType.h"

#include "SFML/Graphics/Rect.hpp"

class Collidable {
public:
	Collidable(ObjectType category);
	void setRect(const sf::FloatRect& rect);
	sf::FloatRect getRect() const;
	bool getActive() const;
	void setActive(bool value);
	int getCategory() const;
	int getFilter() const;
	virtual void collision(ObjectType otherType, Collidable* other);
protected:
	bool m_active;
	sf::FloatRect m_rect;
	int m_category;
	int m_filter;
};
