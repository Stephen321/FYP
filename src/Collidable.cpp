#include "Collidable.h"


#include "Constants.h"
#include "Helpers.h"

using namespace Constants;
using namespace Helpers;

Collidable::Collidable(ObjectType category)
	: m_category((int)category)
	, m_active(true)
{
	switch (category)
	{
	case ObjectType::Unit:
		m_filter = UNIT_COL_FILTERS;
		break;
	case ObjectType::Wall:
		m_filter = WALL_COL_FILTERS;
		break;
	case ObjectType::Bullet:
		m_filter = BULLET_COL_FILTERS;
		break;
	case ObjectType::Ammo:
		m_filter = AMMO_COL_FILTERS;
		break;
	}
}

void Collidable::setRect(const sf::FloatRect & rect)
{
	m_rect = rect;
}

sf::FloatRect Collidable::getRect() const
{
	return m_rect;
}

bool Collidable::getActive() const
{
	return m_active;
}

void Collidable::setActive(bool value)
{
	m_active = value;
}

int Collidable::getCategory() const
{
	return m_category;
}

int Collidable::getFilter() const
{
	return m_filter;
}

void Collidable::collision(ObjectType otherType, Collidable * other)
{
	//std::cout << objectTypeToString((ObjectType)m_category) << " collision with: " << objectTypeToString((ObjectType)other->getCategory()) << std::endl;
}
