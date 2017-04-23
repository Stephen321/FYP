#include "AmmoPool.h"

#include "Helpers.h"

using namespace Helpers;

AmmoPool::AmmoPool(int size, const sf::Texture & texture, Graph * graph)
	: m_size(size)
	, m_graph(graph)
	, m_timer(0.f)
{
	m_ammo = std::vector<Ammo>(m_size, Ammo(texture));
}

void AmmoPool::spawn(const sf::Vector2f & position, int nodeIndex)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_ammo[i].getActive() == false)
		{//found unused ammo
			m_ammo[i].spawn(position, nodeIndex);
			break;
		}
	}
}

void AmmoPool::update(float dt) 
{
	m_timer += dt;
	if (m_timer > SPAWN_RATE)
	{
		m_timer = 0.f;
		int randIndex = m_graph->getRandomIndex();
		sf::Vector2f position = m_graph->getPositionOfNode(randIndex);
		spawn(position, randIndex);
	}
	for (int i = 0; i < m_size; i++)
	{
		m_ammo[i].update(dt);
	}
}

void AmmoPool::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i < m_size; i++)
	{
		target.draw(m_ammo[i]);
	}
}

std::vector<Ammo>& AmmoPool::getAmmo()
{
	return m_ammo;
}

int AmmoPool::getClosestAmmoNode(const sf::Vector2f & position)
{
	int nodeIndex = -1;
	float dist = std::numeric_limits<float>::max();
	for (int i = 0; i < m_size; i++)
	{
		float l = getLength(m_ammo[i].getPosition() - position);
		if (l < dist) 
		{
			dist = l;
			nodeIndex = m_ammo[i].getNodeIndex();
		}
	}
	return nodeIndex;
}
