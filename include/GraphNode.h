#pragma once

#include <vector>
#include "SFML\System\Vector2.hpp"
#include "GraphArc.h"
#include "Cover.h"
#include <unordered_map>

class GraphNode {
public:
	GraphNode() {};
	GraphNode(int index, sf::Vector2f position, int coverType);
	void reset();
	std::vector<GraphArc> getArcs() const;
	int getIndex() const;
	void setGCost(int g);
	void setHCost(int h);
	int getGCost() const;
	int getHCost() const;
	int getFCost() const;
	void setClosed(bool closed);
	bool getClosed() const;
	void setOpen(bool open);
	bool getOpen() const;
	GraphNode* getPrevious() const;
	void setPrevious(GraphNode* previous);
	sf::Vector2f getPosition() const;
	void addArc(const GraphArc& arc);
	std::unordered_map<std::string, Cover>& getCoverSlots();
	const std::unordered_map<std::string, Cover>& getCoverSlots() const;
	int getCoverType() const;
	bool allCoverOccupied() const;
	bool hasCoverFromDirection(const std::string& coverFromDirection);

	//debug
	bool marked;
private:
	sf::Vector2f m_position;
	std::vector<GraphArc> m_arcs;
	int m_index;
	int m_gCost;
	int m_hCost;
	int m_fCost;
	bool m_closed;
	bool m_open; 

	int m_coverType;
	std::unordered_map<std::string, Cover> m_coverSlots;

	GraphNode* m_previous;
};
