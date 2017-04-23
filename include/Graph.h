#pragma once

#include "GraphNode.h"
#include "CollisionManager.h"

#include "SFML\Graphics.hpp"

class Graph : public sf::Drawable{
public:
	Graph() {};
	Graph(int size);
	~Graph();
	void addCollisionManager(const CollisionManager* collisionManager);
	void addNode(int index, sf::Vector2f position, int coverType);
	void addArcs(std::vector<std::pair<int, int>> arcs);
	void setCoverOccupied(int coverIndex, const std::string& coverSlot, bool value);
	std::vector<sf::Vector2f> getPath(int start, int goal);
	std::vector<sf::Vector2f> getPath(sf::Vector2f position, int goal);
	std::pair<float, std::vector<sf::Vector2f>> getPathWithCost(int start, int goal);
	std::pair<float, std::vector<sf::Vector2f>> getPathWithCost(sf::Vector2f position, int goal);
	int getSize() const;
	std::pair<std::pair<int, std::string>, std::vector<sf::Vector2f>> getPathToCover(const std::vector<sf::Vector2f>& enemyPositions, const sf::Vector2f& position, std::string coverFromDirection, const std::pair<int, std::string>& lastCoverSlot, bool flanking = false);
	//std::vector<sf::Vector2f> getPathToCover(const sf::Vector2f& position, int goal);
	sf::Vector2f getPositionOfNode(int index);
	int getRandomIndex();
	void reset();
	bool raycast(const sf::Vector2f& from, const sf::Vector2f& to) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	int calculateCost(int from, int to);
	int getIndexOfClosest(const sf::Vector2f& position);

	typedef std::pair<int, int> NodeCost;
	class NodeCostComparer {
	public:
		bool operator()(const NodeCost& cost1, const NodeCost& cost2)
		{
			return cost1.second > cost2.second;
		}
	};
	int m_size;
	std::vector<GraphNode> m_nodes;
	std::vector<int> m_nonCoverNodes;

	const CollisionManager* m_collisionManager;

	std::vector<sf::CircleShape> debugCircles;
	std::vector<sf::RectangleShape> debugRects;
};
