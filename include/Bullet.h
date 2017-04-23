#pragma once

#include "GameObject.h"

class Bullet : public GameObject {
public:
	enum class Type {
		Pistol,
		Shotgun,
		Rifle,
		Sniper
	};
	Bullet(const sf::Texture& texture);
	void fire(const sf::Vector2f& start, const sf::Vector2f& target, float moveSpeed, int damage, int range);
	void update(float dt) override;	
	void collision(ObjectType otherType, Collidable* other) override;
	int getDamage() const;

protected:
	float m_moveSpeed;
	int m_damage;
	int m_range;
	const float FADE_TIME = 0.25f;
	float m_timer;
	sf::Vector2f m_direction;
	float m_travelledDistance;
};
