#include "CollisionManager.h"

#include "Helpers.h"
#include "SFML\Graphics\RectangleShape.hpp"
#include "GameData.h"

using namespace Helpers;

CollisionManager::CollisionManager()
{
}

void CollisionManager::addCollidable(Collidable * collidable)
{
	m_collidables.push_back(collidable);
}

void CollisionManager::update()
{
	for (int i = 0; i < m_collidables.size(); i++)
	{
		Collidable* collider = m_collidables[i];

		if (collider->getActive())
		{
			for (int j = i + 1; j < m_collidables.size(); j++)
			{
				Collidable* other = m_collidables[j];
				if (other->getActive() && ((collider->getCategory() & other->getFilter()) != (int)ObjectType::None))
				{ //can collide
					if (rectCollision(collider->getRect(), other->getRect()))
					{
						ObjectType otherType = (ObjectType)other->getCategory();
						collider->collision(otherType, other);

						ObjectType colliderType = (ObjectType)collider->getCategory();
						other->collision(colliderType, collider);
					}
				}
			}
		}
	}
}


//http://stackoverflow.com/a/306379
bool CollisionManager::rectCollision(sf::FloatRect A, sf::FloatRect B)
{
	bool xOverlap = valueInRange(A.left, B.left, B.left + B.width ) ||
					valueInRange(B.left, A.left, A.left + A.width );

	bool yOverlap = valueInRange(A.top,  B.top,  B.top  + B.height) ||
					valueInRange(B.top,  A.top,  A.top  + A.height);

	return xOverlap && yOverlap;
}

//http://stackoverflow.com/a/100165
bool CollisionManager::lineRectCollision(sf::FloatRect rect, const sf::Vector2f& lineStart, const sf::Vector2f& lineEnd) const
{
	// Find min and max X for the segment

	double minX = lineStart.x;
	double maxX = lineEnd.x;

	if (lineStart.x > lineEnd.x)
	{
		minX = lineEnd.x;
		maxX = lineStart.x;
	}

	// Find the intersection of the segment's and rectangle's x-projections

	if (maxX > rect.left + rect.width)
	{
		maxX = rect.left + rect.width;
	}

	if (minX < rect.left)
	{
		minX = rect.left;
	}

	if (minX > maxX) // If their projections do not intersect return false
	{
		return false;
	}

	// Find corresponding min and max Y for min and max X we found before

	double minY = lineStart.y;
	double maxY = lineEnd.y;

	double dx = lineEnd.x - lineStart.x;

	if (abs(dx) > 0.0000001)
	{
		double a = (lineEnd.y - lineStart.y) / dx;
		double b = lineStart.y - a * lineStart.x;
		minY = a * minX + b;
		maxY = a * maxX + b;
	}

	if (minY > maxY)
	{
		double tmp = maxY;
		maxY = minY;
		minY = tmp;
	}

	// Find the intersection of the segment's and rectangle's y-projections

	if (maxY > rect.top + rect.height)
	{
		maxY = rect.top + rect.height;
	}

	if (minY < rect.top)
	{
		minY = rect.top;
	}

	if (minY > maxY) // If Y-projections do not intersect return false
	{
		return false;
	}

	return true;
}


void CollisionManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (GameData::Instance().debug == false)
	{
		return;
	}
	for (int i = 0; i < m_collidables.size(); i++)
	{
		sf::FloatRect rect = m_collidables[i]->getRect();
		sf::RectangleShape r;
		r.setPosition(rect.left, rect.top);
		r.setSize(sf::Vector2f(rect.width, rect.height));
		r.setFillColor(sf::Color::Transparent);
		r.setOutlineColor(sf::Color::Red);
		r.setOutlineThickness(3.f);
		target.draw(r);
	}
}

bool CollisionManager::raycast(const sf::Vector2f & from, const sf::Vector2f & to) const
{
	for (int i = 0; i < m_collidables.size(); i++)
	{
		if ((ObjectType)m_collidables[i]->getCategory() == ObjectType::Wall)
		{
			if (lineRectCollision(m_collidables[i]->getRect(), from, to))
				return true;
		}
	}
	return false;
}
