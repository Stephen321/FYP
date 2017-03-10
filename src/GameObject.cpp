#include "GameObject.h"

GameObject::GameObject(Type type, const sf::Vector2f& startPos, const sf::Vector2f& worldSize)
	: m_worldSize(worldSize)
	, m_moving(false)
	, m_type(type)
	, m_position(sf::Vector2f(startPos.x, startPos.y))
	, m_forceAmount(0.f)
	, m_dragCoefficent(0.f)
	, m_maxVelocity(0.f)
	, m_dir()
	, m_active(true)
{
}

void GameObject::setOrigin()
{
	m_sprite.setOrigin(m_sprite.getLocalBounds().width * 0.5f, m_sprite.getLocalBounds().height * 0.5f);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}

void GameObject::update(float dt)
{
	if (m_active == false)
	{
		return;
	}
	move(dt);
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

sf::Vector2f GameObject::getVelocity() const
{
	return m_velocity;
}

void GameObject::setVelocity(const sf::Vector2f & v)
{
	m_velocity = v;
}

void GameObject::moveBy(float dx, float dy)
{
	m_position.x += dx;
	m_position.y += dy;
	m_sprite.setPosition(m_position);
}

GameObject::Type GameObject::getType() const
{
	return m_type;
}

bool GameObject::getActive() const
{
	return m_active;
}

void GameObject::move(float dt)
{
	sf::Vector2f acceleration = calculateAcceleration();
	
	m_velocity += acceleration * dt; //v = u + at
	Helpers::limit(m_velocity, m_maxVelocity);

	m_position += m_velocity * dt + (0.5f * (acceleration * (dt * dt))); // s = ut + 0.5at^2
	checkWorldBounds();
}

sf::Vector2f GameObject::calculateAcceleration()
{
	sf::Vector2f acceleration;

	if (m_moving == false)
	{
		if (abs(m_velocity.x) <= MIN_VEL && m_velocity.x != 0.f)
		{
			m_velocity.x = 0.f;
		}
		if (abs(m_velocity.y) <= MIN_VEL && m_velocity.y != 0.f)
		{
			m_velocity.y = 0.f;
		}
	}
	if (abs(m_velocity.x) > MIN_VEL)
	{
		acceleration.x = m_dragCoefficent * -m_velocity.x;
	}
	if (abs(m_velocity.y) > MIN_VEL)
	{
		acceleration.y = m_dragCoefficent * -m_velocity.y;
	}
	float force = (m_moving) ? m_forceAmount : 0.f;
	acceleration += (force * m_dir); //a = F/m
	return acceleration;
}

void GameObject::checkWorldBounds()
{
	float halfWidth = m_sprite.getGlobalBounds().width * 0.5f;
	float halfHeight = m_sprite.getGlobalBounds().height * 0.5f;
	if (m_position.y < halfHeight)
	{
		m_position.y = halfHeight;
		m_velocity.y = 0.f;
	}		
	else if (m_position.y > m_worldSize.y - halfHeight)
	{
		m_position.y = m_worldSize.y - halfHeight;
		m_velocity.y = 0.f;
	}
	if (m_position.x < -halfWidth)
	{
		m_position.x = m_worldSize.x - halfWidth;
	}
	else if (m_position.x > m_worldSize.x + halfWidth)
	{
		m_position.x = halfWidth;
	}
}

float GameObject::getWidth() const
{
	return m_sprite.getGlobalBounds().width;
}

sf::FloatRect GameObject::getRect() const
{
	float width = m_sprite.getGlobalBounds().width;
	float height = m_sprite.getGlobalBounds().height;
	return sf::FloatRect(m_position.x - width * 0.5f, m_position.y - height * 0.5f, width, height);
}

void GameObject::setDirection(const sf::Vector2f & dir)
{
	m_dir = dir;
	Helpers::normalise(m_dir);
}

float GameObject::getHeight() const
{
	return m_sprite.getGlobalBounds().height;
}

void GameObject::setActive(bool value)
{
	m_active = value;
}

bool GameObject::collision(const std::shared_ptr<GameObject>& collidor)
{
	bool collided = false;
	if (intersects(collidor->getRect()))
	{//collision 
		collided = true;
		m_active = false;

	}
	return collided;
}

bool GameObject::intersects(const sf::FloatRect& r2)
{
	sf::FloatRect r1 = getRect();
	if (r1.left < r2.left + r2.width && r1.left + r1.width > r2.left &&
		r1.top < r2.top + r2.height && r1.top + r1.height > r2.top)
	{
		return true;
	}
	else
	{
		return false;
	}
}
