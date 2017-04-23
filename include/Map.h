#pragma once

#include "SFML\Graphics\VertexArray.hpp"
#include "SFML\Graphics\Texture.hpp"
#include "SFML\Graphics\Drawable.hpp"
#include "SFML\Graphics\RenderTarget.hpp"
#include "SFML\Graphics\Rect.hpp"
#include "Graph.h"
#include "Collidable.h"
#include "CollisionManager.h"

//contains tiles and can draw them
//contains graph used for pathfinding the map
class Map : public sf::Drawable{
public:
	void addVertices(const sf::VertexArray& vertices, const sf::Texture& tileset);
	void addCollidables(const std::vector<Collidable>& collidables);
	void addGraph(const Graph& graph);
	void addCollisionManager(const CollisionManager* collisionManager);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Graph* getGraph();
	std::vector<Collidable>& getCollidables();
	void reset();
private:
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
	std::vector<Collidable> m_collidables;
	Graph m_graph;
};