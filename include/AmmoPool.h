#pragma once

#include "Ammo.h"
#include "Graph.h"

class AmmoPool : public sf::Drawable {
public:
	AmmoPool() {};
	AmmoPool(int size, const sf::Texture& texture, Graph* graph);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	std::vector<Ammo>& getAmmo();
	int getClosestAmmoNode(const sf::Vector2f& position);

private:
	void spawn(const sf::Vector2f& position, int nodeIndex);
	int m_size;
	std::vector<Ammo> m_ammo;
	Graph* m_graph;
	float m_timer;
	static const int SPAWN_RATE = 3;
};