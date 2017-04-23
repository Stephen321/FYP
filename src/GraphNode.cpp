#include "GraphNode.h"

#include "Constants.h"
#include <numeric>

using namespace Constants;

GraphNode::GraphNode(int index, sf::Vector2f position, int coverType)
	: m_index(index)
	, m_position(position)
	, m_coverType(coverType)
	, m_gCost(std::numeric_limits<int>::max())
	, m_hCost(std::numeric_limits<int>::max())
	, m_fCost(std::numeric_limits<int>::max())
	, m_closed(false)
	, m_open(false)
	, m_previous(nullptr)
{
	bool coverDirections[4];
	coverDirections[0] = false;
	coverDirections[1] = false;
	coverDirections[2] = false;
	coverDirections[3] = false;

 	if (m_coverType != -1)
	{
  		if (m_coverType == 0 || m_coverType == 1 || m_coverType == 3 || m_coverType == 4 || m_coverType == 7) //m_coverTypes that have west as true
		{
			m_coverSlots[COVER_WEST] = Cover();
			m_coverSlots[COVER_WEST].position = m_position + sf::Vector2f(-TILE_SIZE, 0.f);
		}
		if (m_coverType == 0 || m_coverType == 2 || m_coverType == 3 || m_coverType == 4 || m_coverType == 5) //m_coverTypes that have north as true
		{
			m_coverSlots[COVER_NORTH] = Cover();
			m_coverSlots[COVER_NORTH].position = m_position + sf::Vector2f(0.f, -TILE_SIZE);
		}
		if (m_coverType == 0 || m_coverType == 1 || m_coverType == 2 || m_coverType == 5 || m_coverType == 6) //m_coverTypes that have east as true
		{
			m_coverSlots[COVER_EAST] = Cover();
			m_coverSlots[COVER_EAST].position = m_position + sf::Vector2f(TILE_SIZE, 0.f);
		}
		if (m_coverType == 1 || m_coverType == 2 || m_coverType == 3 || m_coverType == 6 || m_coverType == 7) //m_coverTypes that have south as true
		{
			m_coverSlots[COVER_SOUTH] = Cover();
			m_coverSlots[COVER_SOUTH].position = m_position + sf::Vector2f(0.f, TILE_SIZE);
		}
	}
}

void GraphNode::reset()
{
	m_gCost = std::numeric_limits<int>::max();
	m_hCost = std::numeric_limits<int>::max();
	m_fCost = std::numeric_limits<int>::max();
	m_closed = false;
	m_open = false;
	m_previous = nullptr;
}

std::vector<GraphArc> GraphNode::getArcs() const
{
	return m_arcs;
}

void GraphNode::setGCost(int g)
{
	m_gCost = g;
	m_fCost = m_gCost + m_hCost;
}

void GraphNode::setHCost(int h)
{
	m_hCost = h;
	m_fCost = m_gCost + m_hCost;
}

int GraphNode::getGCost() const
{
	return m_gCost;
}

int GraphNode::getHCost() const
{
	return m_hCost;
}

int GraphNode::getFCost() const
{
	return m_fCost;
}

void GraphNode::setClosed(bool closed)
{
	m_closed = closed;
}

bool GraphNode::getClosed() const
{
	return m_closed;
}

void GraphNode::setOpen(bool open)
{
	m_open = open;
}

bool GraphNode::getOpen() const
{
	return m_open;
}

GraphNode * GraphNode::getPrevious() const
{
	return m_previous;
}

void GraphNode::setPrevious(GraphNode * previous)
{
	m_previous = previous;
}

int GraphNode::getIndex() const
{
	return m_index;
}

sf::Vector2f GraphNode::getPosition() const
{
	return m_position;
}

void GraphNode::addArc(const GraphArc & arc)
{
	m_arcs.push_back(arc);
}

std::unordered_map<std::string, Cover>& GraphNode::getCoverSlots()
{
	return m_coverSlots;
}

const std::unordered_map<std::string, Cover>& GraphNode::getCoverSlots() const
{
	return m_coverSlots;
}

int GraphNode::getCoverType() const
{
	return m_coverType;
}

bool GraphNode::allCoverOccupied() const
{
	for (auto it = m_coverSlots.begin(); it != m_coverSlots.end(); ++it)
	{
		if (it->second.occupied == false)
		{
			return false;
		}
	}
	return true;
}

bool GraphNode::hasCoverFromDirection(const std::string & coverFromDirection)
{
	return (m_coverSlots.find(coverFromDirection) != m_coverSlots.end()) ? !m_coverSlots[coverFromDirection].occupied : false;
}
