#pragma once
#include "SFML\Graphics.hpp"

#include "SFML/Graphics.hpp" 

class HealthBar : public sf::Drawable
{
public:
	HealthBar();
	HealthBar(float maxHealth);
	bool changeHealth(float change);
	void update(const sf::Vector2f& position);
	void reset();
	void setYOffset(float yOffset);
	void setXSize(float xSize);
	void setYSize(float ySize);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::RectangleShape m_backgroundRect;
	sf::RectangleShape m_healthRect;
	float m_yOffset;
	float m_maxHealth;
	float m_currentHealth;
};