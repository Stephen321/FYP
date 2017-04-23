#pragma once

#include "SFML\Graphics.hpp"
#include "Unit.h"

class HUD : public sf::Drawable{
public:
	HUD(std::vector<Unit>& units);
	void init(int windowWidth, const sf::Texture& portraitATexture, const sf::Texture& portraitBTexture, const sf::Texture& deadTexture, const sf::Texture& bulletsTexture, const sf::Texture& coverTexture, const sf::Font& font);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	struct Portrait {
		sf::Sprite sprite;
		sf::Sprite dead;
		sf::Sprite bullets;
		sf::Sprite cover;
		sf::RectangleShape backgroundRect;
		sf::RectangleShape healthRect;
		std::string name;
		sf::Text healthText;
		int unitIndex;
		sf::Text id;
	};
	const int BULLET_OFFSET = 6;
	std::vector<Unit>& m_units;
	std::vector<Portrait> m_portraits;
	
};
