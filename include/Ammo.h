#pragma once

#include "GameObject.h"

class Ammo : public GameObject {
public:
	Ammo(const sf::Texture & texture);
	void update(float dt) override;
	void spawn(const sf::Vector2f& position, int nodeIndex);
	int getAmmo() const;
	void collision(ObjectType otherType, Collidable * other) override;
	int getNodeIndex() const;

private:
	float m_timer;
	const int TTL = 15;
	int m_ammo;
	int m_nodeIndex;
};
