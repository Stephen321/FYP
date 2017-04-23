#include "Graph.h"

#include <queue>
#include "Constants.h"
#include "Helpers.h"
#include "GameData.h"



using namespace Constants;
using namespace Helpers;


Graph::Graph(int size)
	: m_size(size)
{
	m_nodes.resize(size);
	debugCircles.resize(size);
}

Graph::~Graph()
{
	//for (int i = 0; i < m_nodes.size(); i++)
	//{
	//	delete m_nodes[i];
	//	m_nodes[i] = nullptr;
	//}
}

void Graph::addCollisionManager(const CollisionManager * collisionManager)
{
	m_collisionManager = collisionManager;
}

void Graph::addNode(int index, sf::Vector2f position, int coverType)
{
	m_nodes[index] = GraphNode(index, position, coverType);

	if (coverType == -1)
		m_nonCoverNodes.push_back(index);
	debugCircles[index] = sf::CircleShape(32.f);
	debugCircles[index].setOrigin(32.f, 32.f);
	debugCircles[index].setPosition(position);
	debugCircles[index].setOutlineColor(sf::Color::White);
	debugCircles[index].setFillColor(sf::Color::Transparent);
	debugCircles[index].setOutlineThickness(3.f);
}

void Graph::addArcs(std::vector<std::pair<int, int>> arcs)
{
	//all nodes have been created so add arc lists
	for (int i = 0; i < arcs.size(); i++)
	{
		bool isCover = m_nodes[arcs[i].first].getCoverType() != -1;
		GraphArc newArc;
		newArc.cost = calculateCost(arcs[i].first, arcs[i].second);
		newArc.endNodeIndex = arcs[i].second;
		m_nodes[arcs[i].first].addArc(newArc);

		//debug
		debugRects.push_back(sf::RectangleShape(sf::Vector2f(newArc.cost + 5.f, 3.f)));
		debugRects.back().setPosition(m_nodes[arcs[i].first].getPosition());
		if (isCover)
			debugRects.back().setFillColor(sf::Color(255,255,0,80));
		sf::Vector2f fromPosition = m_nodes[arcs[i].first].getPosition();
		sf::Vector2f toPosition = m_nodes[arcs[i].second].getPosition();
		sf::Vector2f v = toPosition - fromPosition;
		float angle = atan2(v.y, v.x) * (180.f / 3.14f);
		debugRects.back().setRotation(angle);
	}
}

void Graph::setCoverOccupied(int coverIndex, const std::string& coverSlot, bool value)
{
	if (coverIndex == -1)
		return;
	if (m_nodes[coverIndex].getCoverSlots().find(coverSlot) != m_nodes[coverIndex].getCoverSlots().end())
		m_nodes[coverIndex].getCoverSlots()[coverSlot].occupied = value;
}

//find a star path between start and goal index
//wont traverse cover nodes unless thats the goal
std::vector<sf::Vector2f> Graph::getPath(int start, int goal)
{
	return getPathWithCost(start, goal).second;
}

std::vector<sf::Vector2f> Graph::getPath(sf::Vector2f position, int goal)
{
	return getPathWithCost(position, goal).second;
}

std::pair<float, std::vector<sf::Vector2f>> Graph::getPathWithCost(int start, int goal)
{
	std::pair<float, std::vector<sf::Vector2f>> data;

	//reset all nodes
	for (int i = 0; i < m_size; i++)
	{
		m_nodes[i].reset();
		m_nodes[i].marked = false;
	}

	m_nodes[start].marked = true;
	m_nodes[goal].marked = true;

	std::priority_queue<NodeCost, std::vector<NodeCost>, NodeCostComparer> pq; //priorty queue storing node index,f cost. sorted by f cost
	m_nodes[start].setGCost(0.f);
	m_nodes[start].setHCost(calculateCost(start, goal));
	pq.push(NodeCost(start, m_nodes[start].getFCost()));

	bool pathFound = false;
	while (pq.empty() == false && pathFound == false)
	{
		int currentIndex = pq.top().first;
		pq.pop();
		GraphNode* current = &m_nodes[currentIndex];
		current->setClosed(true);
		if (current->getIndex() == goal)
		{
			data.first = current->getFCost();
			for (; current != nullptr; current = current->getPrevious())
			{
				if (current->getCoverType() == -1) //dont push back position to cover node if that was the goal
					data.second.push_back(current->getPosition());
			}
			return data;
		}

		//loop through connected nodes
		std::vector<GraphArc> arcs = current->getArcs();
		for (int i = 0; i < arcs.size(); i++)
		{
			GraphNode* neighbour = &m_nodes[arcs[i].endNodeIndex];

			//ignore
			if (neighbour->getClosed() ||
				neighbour == current->getPrevious() ||
				(neighbour->getCoverType() != -1 && neighbour->getIndex() != goal)) //cant use cover nodes to move unless moving to goal cover
																					//neighbour->walkable() == false)
			{
				continue;
			}

			int tenativeGCost = current->getGCost() + arcs[i].cost;
			if (tenativeGCost < neighbour->getGCost()) {
				//better path
				neighbour->setPrevious(current);
				neighbour->setGCost(tenativeGCost);
				neighbour->setHCost(calculateCost(currentIndex, goal));
			}

			if (neighbour->getOpen() == false) { //not explored
				neighbour->setOpen(true);
				pq.push(NodeCost(neighbour->getIndex(), neighbour->getFCost()));
			}
		}
	}

	//no path found


	return data;
}

std::pair<float, std::vector<sf::Vector2f>> Graph::getPathWithCost(sf::Vector2f position, int goal)
{
	int start = getIndexOfClosest(position);
	return getPathWithCost(start, goal);
}

int Graph::getSize() const
{
	return m_size;
}

//return pair of pair and vector, inner pair is coverIndex and slot name, vector is path of vector2fs
//if lastCoverIndex is not -1 then dont use the same cover node as last time
std::pair<std::pair<int, std::string>, std::vector<sf::Vector2f>> Graph::getPathToCover(const std::vector<sf::Vector2f>& enemyPositions, const sf::Vector2f& position, std::string coverFromDirection, const std::pair<int, std::string>& lastCoverSlot, bool flanking)
{
	std::pair<std::pair<int, std::string>, std::vector<sf::Vector2f>> data;
	data.first.second = "";

	//find nearest cover
	int coverIndex = -1;
	sf::Vector2f coverSlotPosition;

	while (data.first.second == "") //while havnt found cover slot
	{
		std::vector<int> coverIndices;

		//get all valid cover indices
		for (int i = 0; i < m_nodes.size(); i++)
		{
			GraphNode& node = m_nodes[i];
			bool valid = true;
			if (node.getCoverType() == -1) //not cover so skip
				valid = false;
			if (valid && node.allCoverOccupied()) //all slots occupied so skip
				valid = false;
			if (valid && coverFromDirection != "" &&
				node.hasCoverFromDirection(coverFromDirection) == false) // doesnt provide cover from the direction we require 
				valid = false;
			if (valid) //is cover
				coverIndices.push_back(i);
		}

		float dist = std::numeric_limits<float>::max();

		//sort by closest first
		std::sort(coverIndices.begin(), coverIndices.end(), [&](int c1, int c2) {
			float dist1 = getLength(m_nodes[c1].getPosition() - position);
			float dist2 = getLength(m_nodes[c2].getPosition() - position);
			return dist1 < dist2;
		});
		sf::Vector2f target;
		if (flanking)
		{
			bool foundCover = false;;
			for (int i = 0; i < enemyPositions.size() && foundCover == false; i++)
			{
				target = enemyPositions[i]; //target is position of enemy we want to flank towards
				float distToTarget = getLength(target - position); //distance to the enemy
					for (int i = 0; i < coverIndices.size(); i++) //find closest cover which moves us towards enemy
					{
						if (coverIndices.size() > 1 && i == 0)
							continue; //dont do the first cover node (the one you are at) if theres more
						GraphNode& node = m_nodes[coverIndices[i]];
						float distToTargetFromCover = getLength(target - node.getPosition());
						if (distToTargetFromCover < distToTarget) //if we were at this cover position, we would be closer to the target
						{
							foundCover = true;
							coverIndex = coverIndices[i];
							break; //no need to check the rest of cover
						}
					}
			}
		}
		else //loop through valid nodes and find the closet which isnt in range of enemies
		{
			for (int i = 0; i < coverIndices.size(); i++) 
			{
				GraphNode& node = m_nodes[coverIndices[i]];
				bool inRangeOfEnemies = false;
				for (int j = 0; j < enemyPositions.size(); j++)
				{
					if (getLength(enemyPositions[j] - node.getPosition()) < 300.f) //dont get cover that is too close to enemies 
					{
						inRangeOfEnemies = true;
						break;
					}
				}
				if (inRangeOfEnemies == false) 
				{
					coverIndex = coverIndices[i];
					break;
				}
				// in range of enemies then continue to next node
			}
		}

		if (coverIndex == -1)
		{
			if (coverFromDirection != "")
			{
				coverFromDirection = ""; //couldnt find cover from a direction so just get to any type of cover
			}
			else
			{
				//no valid cover remaning
				return data;
			}
		}
		else
		{
			//1. set cover index
			data.first.first = coverIndex;

			//find cover positions possible
			std::unordered_map<std::string, Cover>& coverSlots = m_nodes[coverIndex].getCoverSlots();
			if (coverFromDirection == "") //no cover direct specified, then get closest slot that isnt occupied
			{
				dist = std::numeric_limits<float>::max();
				for (auto it = coverSlots.begin(); it != coverSlots.end(); ++it)
				{
					if (it->second.occupied == false)
					{
						if ((coverIndex == lastCoverSlot.first && it->first == lastCoverSlot.second) == false) //cant go to the same cover slot in same node
						{
							float l = getLength(it->second.position - position);
							if (l < dist)
							{
								dist = l;
								coverSlotPosition = it->second.position;
								//2. set cover slot
								data.first.second = it->first;
							}
						}
					}
				}
			}
			else
			{
				if (coverSlots[coverFromDirection].occupied == false)
				{
					coverSlotPosition = coverSlots[coverFromDirection].position;
					//2. set cover slot
					data.first.second = coverFromDirection;
				}
			}
		}
	}
	

	std::vector<int> neighbourIndices; //get the indices which allow you to enter this cover
	for (int i = 0; i < m_nodes[coverIndex].getArcs().size(); i++)
	{
		GraphArc arc = m_nodes[coverIndex].getArcs()[i];
		neighbourIndices.push_back(arc.endNodeIndex);
	}
	//now we have the cover slot position we want to get to
	//we also know the nodes that allow access to the cover 


	//dont know which nodes allow access to this particular cover slot

	//ray cast from all nodes to the cover slot, ignore ones that intersect the wall
	for (auto it = neighbourIndices.begin(); it != neighbourIndices.end();)
	{
		if (m_collisionManager->raycast(m_nodes[(*it)].getPosition(), coverSlotPosition))
		{ //collides so erase from potential neighbours
			it = neighbourIndices.erase(it);
		}
		else
		{
			++it;
		}
	}

	//no neighbours means its impossible to get to this cover from the node
	if (neighbourIndices.empty())
	{
		data.first.first = -1;
		data.first.second = "";
		return data;
	}

	//neighbour indices now contains the node indices which allow us to enter the cover slot


	//have to find best one if there is more than one
	//int targetNode = neighbourIndices[0];

	std::pair<float, std::vector<sf::Vector2f>> bestCostPath;
	bestCostPath.first = std::numeric_limits<float>::max();
	for (int i = 0; i < neighbourIndices.size(); i++)
	{ //find which node will be the best to take
	  //get index of closest node
		int start = getIndexOfClosest(position);
		//-----------------------------------
		std::pair<float, std::vector<sf::Vector2f>> costPath = getPathWithCost(start, neighbourIndices[i]);
		costPath.first += getLength(m_nodes[neighbourIndices[i]].getPosition() - coverSlotPosition); // add final part of path cost into consideration
		if (costPath.first < bestCostPath.first)
		{
			bestCostPath = costPath;
		}
	}

	//3. set path to cover
	data.second = getPath(position, coverIndex);
	data.second = bestCostPath.second;

	if (data.second.empty() == false)
	{
		//4. update path to go to the slot position and not the node position
		//data.second.erase(data.second.begin()); //remove first
		data.second.insert(data.second.begin(), coverSlotPosition);//insert slot position to move to
	}

	return data;
}

sf::Vector2f Graph::getPositionOfNode(int index)
{
	return m_nodes[index].getPosition();
}

int Graph::getRandomIndex()
{
	int randIndex = m_nonCoverNodes[rand() % m_nonCoverNodes.size()];
	return randIndex; //valid rand index
}

void Graph::reset()
{
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i].getCoverType() != -1)
		{
			setCoverOccupied(i, COVER_WEST, false);
			setCoverOccupied(i, COVER_NORTH, false);
			setCoverOccupied(i, COVER_EAST, false);
			setCoverOccupied(i, COVER_SOUTH, false);
		}
	}
}

bool Graph::raycast(const sf::Vector2f& from, const sf::Vector2f& to) const
{
	return m_collisionManager->raycast(from, to);
}

void Graph::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (GameData::Instance().debug == false)
		return;
	for (int i = 0; i < debugCircles.size(); i++)
	{
		sf::CircleShape c = debugCircles[i];
		if (m_nodes[i].marked)
		{
			c.setFillColor(sf::Color::Green);
		}
		target.draw(c);
	}
	for (const sf::RectangleShape& d : debugRects)
		target.draw(d);

	for (int i = 0; i < m_nodes.size(); i++)
	{
		const std::unordered_map<std::string, Cover>& coverSlots = m_nodes[i].getCoverSlots();
		for (auto it = coverSlots.begin(); it != coverSlots.end(); ++it)
		{
			sf::RectangleShape r;
			r.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
			r.setOrigin(TILE_SIZE * 0.5f, TILE_SIZE * 0.5f);
			r.setOutlineColor(sf::Color::Yellow);
			r.setFillColor(sf::Color(255, 255, 0, 100));
			if (it->second.occupied)
				r.setFillColor(sf::Color(255, 0, 0, 150));
			r.setOutlineThickness(1.f);
			r.setPosition(it->second.position);
			target.draw(r);
		}
	}
}

int Graph::calculateCost(int from, int to)
{
	int cost;
	sf::Vector2f fromPosition = m_nodes[from].getPosition();
	sf::Vector2f toPosition = m_nodes[to].getPosition();
	sf::Vector2f v = toPosition - fromPosition;
	float distance = getLength(v);
	cost = (int)distance / 1.f; 
	return cost;
}

int Graph::getIndexOfClosest(const sf::Vector2f & position)
{
	int index = 0;
	float dist = std::numeric_limits<float>::max();
	for (int i = 0; i < m_size; i++)
	{
		float l = getLength(m_nodes[i].getPosition() - position);
		if (m_nodes[i].getCoverType() == -1 && l < dist) //closest has to not be cover 
		{
			if (m_collisionManager->raycast(position, m_nodes[i].getPosition()) == false) //and also must not cross over a wall
			{
				dist = l;
				index = i;
			}
		}
	}
	return index;
}
