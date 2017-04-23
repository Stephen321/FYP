#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

#include "Helpers.h"
#include "GameData.h"
#include "Collidable.h"
#include "ObjectType.h"

class GameObject : public sf::Drawable, public Collidable
{
public:
	virtual void update(float dt);

	GameObject() : Collidable(ObjectType::None), m_type(ObjectType::None) {};
	GameObject(ObjectType type);
	GameObject(ObjectType type, const sf::Vector2f& startPos);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& v);
	ObjectType getType() const;
	virtual sf::FloatRect getSpriteRect() const;


protected:
	void setOriginToCentre();
	void setOrigin();
	ObjectType m_type;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
};
