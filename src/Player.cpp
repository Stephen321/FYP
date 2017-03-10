#include "Player.h"

Player::Player(const sf::Vector2f& startPos, const sf::Vector2f& worldSize)
	: GameObject(Type::Player, startPos, worldSize)
	, m_reloadTimer(0.f)
	, m_score(0)
	, m_liveTimer (0.f)
{
	m_dragCoefficent = 0.8f;
	m_maxVelocity = 200.f;
	m_healthBar = HealthBar(100);
	m_healthBar.setYOffset(-(m_sprite.getGlobalBounds().height * 0.5f + HEALTH_Y_OFFSET));
	m_healthBar.setXSize(m_sprite.getGlobalBounds().width);
	m_dir.x = 1.f;
	m_canHyperJump = true;
	m_bombReady = true;

	setOrigin();
}

void Player::update(float dt)
{
	m_liveTimer += dt;
	if (m_liveTimer > 1.f)
	{
		m_liveTimer = 0.f;
		m_score += 10.f;
	}
	checkInput();
	if (m_reloadTimer < RELOAD_TIME)
	{
		m_reloadTimer += dt;
	}
	if (m_smartBombTimer > 0)
	{
		m_smartBombTimer -= dt;
	}
	else
		m_bombReady = true;
	m_healthBar.update(m_position);
	GameObject::update(dt);
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_healthBar);
	GameObject::draw(target, states);
}

bool Player::collision(const std::shared_ptr<GameObject>& collidor)
{
	bool collided = false;
	if (GameObject::collision(collidor))
	{
		collided = true;

		float damage = 0.f;
		if (m_healthBar.changeHealth(-damage) == false)
		{
		}
	}
	return collided;
}

int Player::getScore()
{
	return m_score;
}

void Player::increaseScore(int value)
{
	m_score += value;
}

void Player::fire()
{
	if (m_reloadTimer < RELOAD_TIME)
		return;

	m_reloadTimer = 0.f;
	sf::Vector2f dir = m_dir;
	if (dir.x < 0.f)
	{
		dir.x = -1.f;
	}
	else if (dir.x > 0.f)
	{
		dir.x = 1.f;
	}
	dir.y = 0.f;
}

void Player::smartBomb()
{
	if (m_smartBombTimer <= 0)
	{
		m_bombReady = false;
		m_smartBombTimer = SMARTBOMB_COOLDOWN;
	}
}

bool Player::BombAvailable() const
{
	return m_bombReady;
}

void Player::hyperJump()
{
	if (m_canHyperJump)
	{
		m_position = sf::Vector2f(Helpers::randomNumber(0, m_worldSize.x), Helpers::randomNumber(0, m_worldSize.y));
		m_velocity = sf::Vector2f();

		if (rand() % 2 == 0)
		{
			m_dir.x *= -1;
			m_sprite.setScale(sf::Vector2f(m_sprite.getScale().x * m_dir.x, 1.f));
		}

		m_canHyperJump = false;
	}	
}

bool Player::canHyperJump() const
{
	return m_canHyperJump;
}

void Player::replenishHyperJump()
{
	m_canHyperJump = true;
}

void Player::checkInput()
{
	m_moving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_dir.x = -1.f;
		m_sprite.setScale(1.f, 1.f);
		m_moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_dir.x = 1.f;
		m_sprite.setScale(-1.f, 1.f);
		m_moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_dir.y = -1.f;
		m_moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_dir.y = 1.f;
		m_moving = true;
	}
	Helpers::normalise(m_dir);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		fire();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
	{
		smartBomb();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
	{
		hyperJump();
	}
}
