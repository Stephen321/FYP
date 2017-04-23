#include "Ammo.h"





Ammo::Ammo(const sf::Texture & texture)
	: GameObject(ObjectType::Ammo)
	, m_ammo(10)
	, m_nodeIndex(-1)
{
	m_sprite.setTexture(texture);
	m_active = false;
	setOriginToCentre();
	setRect(getSpriteRect());
}

void Ammo::update(float dt)
{
	if (m_active == false)
	{
		return;
	}
	m_timer += dt;
	if (m_timer > TTL)
	{
		m_active = false;
		m_nodeIndex = -1;
	}
	GameObject::update(dt);
}

void Ammo::spawn(const sf::Vector2f & position, int nodeIndex)
{
	m_active = true;
	m_position = position;
	m_sprite.setPosition(position);
	setRect(getSpriteRect());
	m_timer = 0.f;
	m_nodeIndex = nodeIndex;
}

int Ammo::getAmmo() const
{
	return m_ammo;
}

void Ammo::collision(ObjectType otherType, Collidable * other)
{
	m_active = false;
	m_nodeIndex = -1;
}

int Ammo::getNodeIndex() const
{
	return m_nodeIndex;
}
