#include "Map.h"

void Map::addVertices(const sf::VertexArray & vertices, const sf::Texture & tileset)
{
	m_vertices = vertices;
	m_tileset = tileset;
}

void Map::addCollidables(const std::vector<Collidable>& collidables)
{
	m_collidables = collidables;
}

void Map::addGraph(const Graph & graph)
{
	//copied 
	//only done once and no memory to worry about in graph
	//graph is used nowhere else
	m_graph = graph; 
}

void Map::addCollisionManager(const CollisionManager* collisionManager)
{
	//dont want to have different copies of the collision manager
	//as collision manager also used in gamescene
	//make sure graph has been added before calling this method **
	// graph is added in game load and this is added in game play scene
	m_graph.addCollisionManager(collisionManager);
}

void Map::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &m_tileset;
	target.draw(m_vertices, states);

	target.draw(m_graph);
}

Graph * Map::getGraph()
{
	return &m_graph;
}

std::vector<Collidable>& Map::getCollidables()
{
	return m_collidables;
}

void Map::reset()
{
	m_graph.reset();
}
