#include "GameObject.h"

GameObject::GameObject(ObjectType type)
	: Collidable(type), m_type(type)
{
}

GameObject::GameObject(ObjectType type, const sf::Vector2f& startPos)
	: Collidable(type)
	, m_type(type)
	, m_position(startPos)
{
}

void GameObject::setOrigin()
{
	m_sprite.setOrigin(m_sprite.getLocalBounds().width * 0.5f, m_sprite.getLocalBounds().height * 0.5f);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_active == false)
	{
		return;
	}
	target.draw(m_sprite);
}

void GameObject::update(float dt)
{
	if (m_active == false)
	{
		return;
	}
	setRect(getSpriteRect()); //update the collidable rect to always be the sprite rect 
	m_sprite.setPosition(m_position);
}

sf::Vector2f GameObject::getPosition() const
{
	return m_position;
}

void GameObject::setPosition(const sf::Vector2f & v)
{
	m_position = v;
	m_sprite.setPosition(m_position);
}

ObjectType GameObject::getType() const
{
	return m_type;
}

sf::FloatRect GameObject::getSpriteRect() const
{
	float width = m_sprite.getGlobalBounds().width;
	float height = m_sprite.getGlobalBounds().height;
	return sf::FloatRect(m_position.x - width * 0.5f, m_position.y - height * 0.5f, width, height);
}

void GameObject::setOriginToCentre()
{
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width * 0.5f, m_sprite.getGlobalBounds().height * 0.5f);
}
