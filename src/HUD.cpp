#include "HUD.h"

#include "Constants.h"

using namespace Constants;

HUD::HUD(std::vector<Unit>& units)
	: m_units(units)
{
}


void HUD::init(int windowWidth, const sf::Texture & portraitATexture, const sf::Texture& portraitBTexture, const sf::Texture& deadTexture, const sf::Texture& bulletsTexture, const sf::Texture& coverTexture, const sf::Font& font)
{
	int edgeOffset = 50;
	int offset = 15;
	int xPos = edgeOffset * 0.5f;
	int yPos = edgeOffset * 0.5f;
	int deadYOffset = 5;
	int healthTextOffset = 9;
	int textLeftOffset = 5;
	int bulletsLeftOffset = 5;
	int bulletsYOffset = 6;
	int coverXOffset = 1;
	int coverYOffset = 2;
	int healthCharSize = 14;
	int idXOffset = 2;

	bool firstTeamB = true;
	for (int i = 0; i < m_units.size(); i++)
	{
		Portrait portrait;
		portrait.id = sf::Text(std::to_string(m_units[i].getID()), font);
		portrait.id.setCharacterSize(healthCharSize * 1.3f);

		portrait.dead = sf::Sprite(deadTexture);
		portrait.bullets = sf::Sprite(bulletsTexture);
		portrait.cover = sf::Sprite(coverTexture);
		portrait.unitIndex = i;
		portrait.healthText = sf::Text("Health: ", font);

		portrait.healthText.setCharacterSize(healthCharSize);
		portrait.healthText.setStyle(sf::Text::Bold);
		portrait.healthText.setFillColor(sf::Color(20, 20, 20));

		int rectWidth = portrait.bullets.getGlobalBounds().width;
		portrait.backgroundRect = sf::RectangleShape(sf::Vector2f(rectWidth, healthCharSize + 2));
		portrait.backgroundRect.setOutlineColor(sf::Color(25, 25, 25, 255));
		portrait.backgroundRect.setFillColor(sf::Color(220, 10, 30));
		portrait.backgroundRect.setOutlineThickness(1.f);

		portrait.healthRect = sf::RectangleShape(sf::Vector2f(rectWidth, healthCharSize + 2));
		portrait.healthRect.setOutlineColor(sf::Color::Transparent);
		portrait.healthRect.setFillColor(sf::Color(0, 170, 0));
		portrait.healthRect.setOutlineThickness(1.f);

		if (m_units[i].getTeam() == TEAM_A)
		{
			portrait.sprite = sf::Sprite(portraitATexture);
		}
		else
		{
			portrait.sprite = sf::Sprite(portraitBTexture);
			if (firstTeamB)
			{
				firstTeamB = false;
				xPos = windowWidth - edgeOffset - ((portrait.sprite.getGlobalBounds().width + offset * 0.5f) * (m_units.size() * 0.5f));
			}
		}
		portrait.sprite.setPosition(xPos, yPos);
		portrait.dead.setPosition(xPos + (portrait.dead.getGlobalBounds().width * 0.5f), yPos + deadYOffset);
		portrait.cover.setPosition(xPos + (portrait.dead.getGlobalBounds().width * 1.5f) + coverXOffset, yPos + deadYOffset + coverYOffset);
		portrait.healthText.setPosition(xPos + (portrait.sprite.getGlobalBounds().width * 0.5f), yPos + deadYOffset + portrait.dead.getGlobalBounds().height + healthTextOffset);

		portrait.id.setPosition(xPos + portrait.id.getGlobalBounds().width + idXOffset, portrait.cover.getPosition().y);

		portrait.bullets.setPosition(xPos + bulletsLeftOffset, portrait.healthText.getPosition().y + portrait.healthText.getGlobalBounds().height + bulletsYOffset);
		portrait.backgroundRect.setPosition(portrait.bullets.getPosition().x + 1, portrait.healthText.getPosition().y);
		portrait.healthRect.setPosition(portrait.bullets.getPosition().x + 1, portrait.healthText.getPosition().y);

		m_portraits.push_back(portrait);
		xPos += portrait.sprite.getGlobalBounds().width + offset;;
	}
}

void HUD::update(float dt)
{
	for (int i = 0; i < m_portraits.size(); i++)
	{
		int health = m_units[m_portraits[i].unitIndex].getCurrentHealth();
		if (health < 0)
			health = 0;
		int maxHealth = m_units[m_portraits[i].unitIndex].getMaxHealth();

		int ammo = m_units[m_portraits[i].unitIndex].getAmmo();

		m_portraits[i].healthText.setString(std::string(std::to_string(health) + " / " + std::to_string(maxHealth)));
		m_portraits[i].healthText.setOrigin(m_portraits[i].healthText.getGlobalBounds().width * 0.5f, 0.f);

		m_portraits[i].healthRect.setSize(sf::Vector2f(((float)health / maxHealth) * m_portraits[i].backgroundRect.getSize().x, m_portraits[i].backgroundRect.getSize().y));

		m_portraits[i].bullets.setTextureRect(sf::IntRect(0, 0, ammo * BULLET_OFFSET, m_portraits[i].bullets.getGlobalBounds().height));
	}
}

void HUD::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i < m_portraits.size(); i++)
	{
		target.draw(m_portraits[i].sprite);
		bool active = m_units[m_portraits[i].unitIndex].getActive();
		if (active == false)
			target.draw(m_portraits[i].dead);
		target.draw(m_portraits[i].backgroundRect);
		target.draw(m_portraits[i].healthRect);
		target.draw(m_portraits[i].healthText);
		target.draw(m_portraits[i].bullets);
		target.draw(m_portraits[i].id);
		if (active && m_units[m_portraits[i].unitIndex].getInCover())
			target.draw(m_portraits[i].cover); 
	}
}
