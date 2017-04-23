#include "BulletPool.h"

#include "GameData.h"


BulletPool::BulletPool(int size, const sf::Texture& texture)
	: m_size(size)
{
	m_bullets = std::vector<Bullet>(m_size, Bullet(texture));
}

void BulletPool::fire(const sf::Vector2f & start, const sf::Vector2f & target, Bullet::Type type, bool fromCover)
{
	GameData& data = GameData::Instance();
	int damage = 0;
	float moveSpeed = 0.f;
	int range = 0;
	switch (type)
	{
	case Bullet::Type::Pistol:
		damage = data.pistolDamage;
		moveSpeed = data.pistolMoveSpeed;
		range = data.pistolRange;
		break;
	case Bullet::Type::Shotgun:
		damage = data.shotgunDamage;
		moveSpeed = data.shotgunMoveSpeed;
		range = data.shotgunRange;
		break;
	case Bullet::Type::Rifle:
		damage = data.rifleDamage;
		moveSpeed = data.rifleMoveSpeed;
		range = data.rifleRange;
		break;
	case Bullet::Type::Sniper:
		damage = data.sniperDamage;
		moveSpeed = data.sniperMoveSpeed;
		range = data.sniperRange;
		break;
	}

	//cover shots travel twice as far
	if (fromCover)
		range *= 2;

	bool foundOne = false;
	for (int i = 0; i < m_size; i++)
	{
		if (m_bullets[i].getActive() == false)
		{//found unused bullet
			m_bullets[i].fire(start, target, moveSpeed, damage, range);
			foundOne = true;
			break;
		}
	}
	if (foundOne == false)
	{
		//Couldnt find a free bullet
	}
}

void BulletPool::update(float dt)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_bullets[i].getActive())
			m_bullets[i].update(dt);
	}
}

void BulletPool::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i < m_size; i++)
	{
		target.draw(m_bullets[i]);
	}
}

std::vector<Bullet>& BulletPool::getBullets()
{
	return m_bullets;
}
