#include "Bullet.h"

#include "Helpers.h"

using namespace Helpers;

Bullet::Bullet(const sf::Texture & texture)
	: GameObject(ObjectType::Bullet)
{
	m_sprite.setTexture(texture);
	m_active = false;
	setOriginToCentre();
	setRect(getSpriteRect());
}

void Bullet::fire(const sf::Vector2f& start, const sf::Vector2f & target, float moveSpeed, int damage, int range)
{
	m_active = true;
	m_position = start;
	m_direction = target - start;
	normalise(m_direction);
	m_moveSpeed = moveSpeed;
	m_damage = damage;
	m_range = range * 4;
	m_travelledDistance = 0.f;
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(getAngle(m_direction));
	m_sprite.setColor(sf::Color(255, 255, 255, 255));
	m_timer = 0.f;
	setRect(getSpriteRect());
}

void Bullet::update(float dt)
{
	if (m_active == false)
	{
		return;
	}
	m_position += m_direction * m_moveSpeed * dt;
	m_travelledDistance += m_moveSpeed * dt;
	if (m_travelledDistance > m_range)
	{
		m_timer += dt;
		m_sprite.setColor(sf::Color(255, 255, 255, lerp(255, 0, m_timer / FADE_TIME)));
		if (m_sprite.getColor().a == 0.f)
		{
			m_active = false;
			m_position.x = -100;
			m_position.y = -100;
			setRect(getSpriteRect());
		}
	}
	GameObject::update(dt);
}

void Bullet::collision(ObjectType otherType, Collidable * other)
{
	m_active = false;
	m_position.x = -100;
	m_position.y = -100;
	setRect(getSpriteRect());
}

int Bullet::getDamage() const
{
	return m_damage;
}
