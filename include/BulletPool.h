#pragma once

#include "Bullet.h"

class BulletPool : public sf::Drawable{
public:
	BulletPool() {};
	BulletPool(int size, const sf::Texture& texture);
	bool fire(const sf::Vector2f& start, const sf::Vector2f& target, Bullet::Type type, bool fromCover = false);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	std::vector<Bullet>& getBullets();

private:
	int m_size;
	std::vector<Bullet> m_bullets;
};