#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

#include "Helpers.h"
#include "GameData.h"

class GameObject : public sf::Drawable
{
public:
	enum class Type {
		Player
	};
	GameObject(Type type, const sf::Vector2f& startPos, const sf::Vector2f& worldSize);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void update(float dt);
	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& v);
	sf::Vector2f getVelocity() const;
	void setVelocity(const sf::Vector2f& v);
	void moveBy(float dx, float dy);
	Type getType() const;
	bool getActive() const;
	virtual void move(float dt);
	virtual void checkWorldBounds();
	float getWidth() const;
	void setDirection(const sf::Vector2f& dir);
	virtual sf::FloatRect getRect() const;
	virtual float getHeight() const;
	void setActive(bool value);
	virtual bool collision(const std::shared_ptr<GameObject>& collidor);
	bool intersects(const sf::FloatRect& r2);

protected:
	void setOrigin();
	sf::Vector2f calculateAcceleration();

	sf::Vector2f m_worldSize;
	bool m_active;
	bool m_moving;
	const float MIN_VEL = 2.5f;
	Type m_type;
	sf::Sprite m_sprite;
	sf::Vector2f m_dir;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_forceAmount;
	float m_dragCoefficent;
	float m_maxVelocity;
};
