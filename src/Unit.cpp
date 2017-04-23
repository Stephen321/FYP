#include "Unit.h"


#include "Helpers.h"
#include "Constants.h"
#include "TakeCoverAction.h"
#include "IdleAction.h"
#include "FireCoverAction.h"
#include "ReloadAction.h"
#include "PickUpAmmoAction.h"
#include "MoveToNodeAction.h"
#include "FlankCoverAction.h"
#include "GameData.h"

using namespace Helpers;
using namespace Constants;

int Unit::ID = 1;

Unit::Unit(const sf::Vector2f& spawnPos, const sf::Texture& texture, Graph* graph, BulletPool* bulletPool, AmmoPool* ammoPool, std::vector<Unit>& otherUnits, const sf::Font& font)
	: GameObject(ObjectType::Unit, spawnPos)
	, m_team(TEAM_A)
	, m_spawnPos(spawnPos)
	, m_graph(graph)
	, m_bulletPool(bulletPool)
	, m_ammoPool(ammoPool)
	, m_otherUnits(otherUnits)
	, m_reloadTimer(0.f)
	, m_timer(0.f)
	, m_timeToTarget(0.f)
	, m_faceDirection(0.f)
	, m_coverIndexSlot(-1, "")
	, m_inCover(false)
	, m_healthTimer(0.f)
	, m_ammoTargetIndex(-1)
	, m_useHard(false)
{
	m_idText.setFont(font);
	m_idText.setCharacterSize(55);

	m_sprite.setTexture(texture);
	setOriginToCentre();
	setRect(getSpriteRect());

	//add actions here
	m_availableActions.push_back(new TakeCoverAction);
	//m_availableActions.push_back(new IdleAction);
	m_availableActions.push_back(new FireCoverAction);
	m_availableActions.push_back(new ReloadAction);
	m_availableActions.push_back(new PickUpAmmoAction);
	m_availableActions.push_back(new MoveToNodeAction);
	m_availableActions.push_back(new FlankCoverAction);

	m_id = ID;

	ID++;
	m_idText.setString(std::to_string(m_id));
	m_idText.setOrigin(m_idText.getGlobalBounds().width * 0.5f, m_idText.getGlobalBounds().height * 0.5f);
	if (ID == 5)
		ID = 1;
}

void Unit::init(int team, float moveSpeed, float reloadTime, int maxHealth, int maxAmmo, int range)
{
	m_team = team;
	m_moveSpeed = moveSpeed;
	m_reloadTime = reloadTime;
	m_maxHealth = maxHealth;
	m_currentHealth = m_maxHealth;
	m_maxAmmo = maxAmmo;
	m_ammo = m_maxAmmo;
	m_range = range;
	if (m_team == TEAM_A)
	{
		m_useHard = (GameData::Instance().aDiff == DIFF_HARD);
		m_idText.setFillColor(sf::Color::Blue);
	}
	else
	{
		m_useHard = (GameData::Instance().bDiff == DIFF_HARD);
		m_idText.setFillColor(sf::Color::Red);
	}
	if (m_useHard == false)
		m_state = GoapAgent::FSMState::None;
}

void Unit::setPath(const std::vector<sf::Vector2f> path)
{
	if (path.empty())
		return;
	m_path = path;
	m_startPos = m_position;
	m_timeToTarget = (getLength(m_path[m_path.size() - 1] - m_position) / m_moveSpeed);
	m_timer = 0.f;
	if (m_inCover)
	{
		removeFromCover();
	}
}

void Unit::applyDamage(int damage)
{
	m_currentHealth -= damage;
	if (m_currentHealth < 0)
	{
		m_active = false;
		m_respawnTimer = 0.f;
	}
}

void Unit::refillAmmo(int ammo)
{
	m_ammo += ammo;
	if (m_ammo > m_maxAmmo)
		m_ammo = m_maxAmmo;
}

void Unit::respawn(float dt)
{
	m_respawnTimer += dt;
	if (m_respawnTimer > RESPAWN_TIME)
	{
		if (m_team == TEAM_A)
			GameData::Instance().teamADeaths++;
		else
			GameData::Instance().teamBDeaths++;
		reset();
	}
}

void Unit::reset()
{
	GameData& data = GameData::Instance();
	if (m_team == TEAM_A)
		data.teamADeaths++;
	else
		data.teamBDeaths++;
	m_position = m_spawnPos;
	m_active = true;
	m_reloadTimer = 0.f;
	m_currentHealth = m_maxHealth;
	m_timer = 0.f;
	m_timeToTarget = 0.f;
	m_faceDirection = 0.f;
	m_path.clear();
	m_sprite.setPosition(m_position);
	setRect(getSpriteRect());
	if (m_useHard)
		m_state = FSMState::Idle;
	m_currentActions.clear();
	if (m_inCover)
	{
		removeFromCover();
	}
	m_lastCoverIndexSlot.first = -1;
}

void Unit::removeFromCover()
{
	if (m_lastCoverIndexSlot.first == -1)
		return;
	m_graph->setCoverOccupied(m_lastCoverIndexSlot.first, m_lastCoverIndexSlot.second, false);
	m_lastCoverIndexSlot.first = -1;
	m_inCover = false;
}

void Unit::update(float dt)
{
	//fsm

	m_ammoTargetIndex = m_ammoPool->getClosestAmmoNode(m_position);

	if (m_active == false)
	{
		respawn(dt);
		return;
	}

	switch (m_state)
	{
	case FSMState::Idle:
	{
		// GOAP planning
		
		State worldState = getWorldState();
		State goal = createGoalState();

		// Plan
		std::vector<GoapAction*> plan = m_planner.plan(this, m_availableActions, worldState, goal);
		if (plan.empty() == false) 
		{
			m_currentActions = plan;
			m_state = FSMState::PerformAction;
		}
		else 
		{
			m_state = FSMState::Idle;
		}
		break;
	}
	case FSMState::MoveTo:
	{
		GoapAction* action = m_currentActions.back();
		if (action->getNeedsInRange() == false)// || action->isDone(this))
		{
			m_state = FSMState::Idle;
			return;
		}

		if (m_path.empty())
		{	//move tos are move to cover, flee cover, move to ammo, move to node
			if (action->getType() == GoapAction::Type::TakeCover)
			{
				//which cover will remove us from the sight of enemies best
				std::vector<int> enemiesInRange = getEnemiesInRange(false, m_range);
				std::vector<int> enemiesInLOS = getEnemiesInRange(true, m_range * 2, 100);
				if (enemiesInLOS.empty() == false)
					enemiesInRange.insert(enemiesInRange.begin(), enemiesInLOS.begin(), enemiesInLOS.end());

				std::string coverDirection = ""; //need to find which direction we need cover from the most
				int westEnemies = 0;
				int northEnemies = 0;
				int eastEnemies = 0;
				int southEnemies = 0;


				for (int i = 0; i < enemiesInRange.size(); i++)
				{
					sf::Vector2f pos = m_otherUnits[enemiesInRange[i]].getPosition();
					if (pos.x < m_position.x)
						westEnemies++;
					else 
						eastEnemies++;
					if (pos.y < m_position.y) 
						northEnemies++;
					else
						southEnemies++;
				}
				int sum = westEnemies + eastEnemies + northEnemies + southEnemies;
				if (sum > 0)
				{
					int randNum = rand() % sum;
					if (randNum < westEnemies)
					{
						coverDirection = COVER_EAST;
					}
					else if (randNum < westEnemies + eastEnemies)
					{
						coverDirection = COVER_WEST;
					}
					else if (randNum < westEnemies + eastEnemies + northEnemies)
					{
						coverDirection = COVER_SOUTH;
					}
					else if (randNum < westEnemies + eastEnemies + northEnemies + southEnemies)
					{
						coverDirection = COVER_NORTH;
					}
				}
				moveToCover(coverDirection);
			}
			else if (action->getType() == GoapAction::Type::PickUpAmmo)
			{
				moveToAmmo();
			}
			else if (action->getType() == GoapAction::Type::MoveToNode)
			{
				moveTo(m_graph->getRandomIndex());
			}
			else if (action->getType() == GoapAction::Type::FlankCover)
			{
				moveToFlankCover();
			}
			if (m_path.empty())
			{
				//failed to get path
				m_state = FSMState::Idle;
			}
		}

		updatePath();
		m_state = FSMState::PerformAction;

		break;
	}
	case FSMState::PerformAction:
	{
		// perform the action

		if (!hasPlan()) {
			// no actions to perform
			m_state = FSMState::Idle;
			return;
		}

		GoapAction* action = m_currentActions.back();
		if (action->isDone(this)) 
		{
			// the action is done. Remove it so we can perform the next one
			m_currentActions.pop_back();
		}

		if (hasPlan())
		{
			//TODO: use vector instead
			// perform the next action
			action = m_currentActions.back();
			if ((action->getNeedsInRange() && action->getInRange()) || action->getNeedsInRange() == false)
			{
				bool success = action->perform(this);

				if (!success) {
					m_state = FSMState::Idle;
				}
			}
			else 
			{
				m_state = FSMState::MoveTo;
			}

		}
		else
		{
			// no actions left, move to Plan state
			m_state = FSMState::Idle;
		}
		break;
	}
	case GoapAgent::FSMState::None:
	{
		updatePath();
		break;
	}

	}


	//stuff below here is updated all the time (timers)

	m_reloadTimer += dt;
	m_timer += dt;
	m_inCoverTimer += dt;

	if (m_inCover)
	{
		m_healthTimer += dt;
		if (m_healthTimer > HEALTH_RECOVER_TIME)
		{
			m_healthTimer = 0.f;
			m_currentHealth += HEALTH_RECOVER_AMOUNT;
			if (m_currentHealth > m_maxHealth)
				m_currentHealth = m_maxHealth;
		}
	}
	//fireAtPosition(m_fireTarget);

	std::vector<int> enemiesInRange;
	if (m_inCover)
	{
		enemiesInRange = getEnemiesInRange(true, m_range * 2, 100);
	}
	else
	{
		enemiesInRange = getEnemiesInRange(true, m_range, 100);
	}
	float dist = std::numeric_limits<float>::max();
	m_fireTarget = sf::Vector2f(-100, -100);
	for (int i = 0; i < enemiesInRange.size(); i++)
	{
		float l = getLength(m_otherUnits[enemiesInRange[i]].getPosition() - m_position);
		if (l < dist)
		{
			m_fireTarget = m_otherUnits[enemiesInRange[i]].getPosition();
		}
	}
	if (m_state == FSMState::MoveTo || m_state == FSMState::None)
	{ //can fire in the middle of  move to state
		fireAtTarget(false);
	}

	sf::Vector2f faceTarget(-100, -100);
	for (int i = 0; i < enemiesInRange.size(); i++)
	{
		float l = getLength(m_otherUnits[enemiesInRange[i]].getPosition() - m_position);
		if (l < dist)
		{
			faceTarget = m_otherUnits[enemiesInRange[i]].getPosition();
		}
	}
	if (faceTarget == sf::Vector2f(-100, -100))
	{
		if (m_path.empty() == false)
		{
			faceTarget = m_path[m_path.size() - 1];
		}
		else if (m_lastCoverIndexSlot.second != "")
		{ //standing in cover
			if (m_lastCoverIndexSlot.second == COVER_WEST)
			{
				faceTarget = m_position + sf::Vector2f(1, 0);
			}
			else if (m_lastCoverIndexSlot.second == COVER_NORTH)
			{
				faceTarget = m_position + sf::Vector2f(0, 1);
			}
			else if (m_lastCoverIndexSlot.second == COVER_EAST)
			{
				faceTarget = m_position + sf::Vector2f(-1, 0);
			}
			else if (m_lastCoverIndexSlot.second == COVER_SOUTH)
			{
				faceTarget = m_position + sf::Vector2f(0, -1);
			}
		}
	}
	 
	m_idText.setPosition(m_position + sf::Vector2f(0, -(ID_TEXT_Y_OFFSET + m_sprite.getGlobalBounds().height * 0.5f) ));
	m_lastAmmoTargetIndex = m_ammoTargetIndex;
	m_faceDirection = getAngle(m_position, faceTarget);
	m_sprite.setRotation(m_faceDirection);
	setRect(getSpriteRect());
	GameObject::update(dt);
}

void Unit::moveToAmmo()
{
	if (m_ammoTargetIndex == -1)
	{
		//no ammo to move to
		return;
	}
	m_coverIndexSlot.first = -1;
	m_coverIndexSlot.second = "";
	removeFromCover();

	setPath(m_graph->getPath(m_position, m_ammoTargetIndex));
}

void Unit::moveTo(int goal)
{
	m_coverIndexSlot.first = -1;
	m_coverIndexSlot.second = "";
	removeFromCover();

	setPath(m_graph->getPath(m_position, goal));
}

void Unit::moveToFlankCover()
{
	if (m_inCover == false)
		m_lastCoverIndexSlot = m_coverIndexSlot;

	std::vector<sf::Vector2f> enemyPositions;

	for (int i = 0; i < m_otherUnits.size(); i++)
	{
		if (m_otherUnits[i].getTeam() != m_team)
		{
			enemyPositions.push_back(m_otherUnits[i].getPosition());
		}
	}


	std::pair<std::pair<int, std::string>, std::vector<sf::Vector2f>> data = m_graph->getPathToCover(enemyPositions, m_position, "", m_lastCoverIndexSlot, true);
	if (data.second.empty() == false)
	{
		removeFromCover();
		m_coverIndexSlot = data.first;
		setPath(data.second);
		m_graph->setCoverOccupied(m_coverIndexSlot.first, m_coverIndexSlot.second, true);
	}
}

//move to cover that isnt near enemies
void Unit::moveToCover(const std::string& coverFromDirection)
{
	if (m_inCover == false)
		m_lastCoverIndexSlot = m_coverIndexSlot;

	std::vector<sf::Vector2f> enemyPositions;

	for (int i = 0; i < m_otherUnits.size(); i++)
	{
		if (m_otherUnits[i].getTeam() != m_team)
		{
			if (m_otherUnits[i].getCoverSlotPosition() != sf::Vector2f(-100, -100))
			{
				enemyPositions.push_back(m_otherUnits[i].getCoverSlotPosition());
			}
		}
	}

	std::pair<std::pair<int, std::string>, std::vector<sf::Vector2f>> data = m_graph->getPathToCover(enemyPositions, m_position, coverFromDirection, m_lastCoverIndexSlot);
	if (data.second.empty() == false)
	{
		removeFromCover();
		m_coverIndexSlot = data.first;

		setPath(data.second);
		m_graph->setCoverOccupied(m_coverIndexSlot.first, m_coverIndexSlot.second, true);
	}
}

void Unit::moveFromTo(int start, int goal)
{
	std::vector<sf::Vector2f> path = m_graph->getPath(m_position, start);
	std::vector<sf::Vector2f> path2 = m_graph->getPath(start, goal);
	path.insert(path.end(), path2.begin(), path2.end());
	setPath(path);
}

void Unit::fireAtTarget(bool fromCover)
{
	if (m_fireTarget.x == -100 && m_fireTarget.y == -100)
		return;
	if (m_reloadTimer > m_reloadTime && m_ammo > 0)
	{
		m_ammo--;
		m_reloadTimer = 0.f;
		sf::Vector2f offset = m_fireTarget - m_position;
		normalise(offset);
		offset *= m_rect.width * 1.5f;
		m_bulletPool->fire(m_position + offset, m_fireTarget, Bullet::Type::Pistol, fromCover);
	}
}

void Unit::collision(ObjectType otherType, Collidable * other)
{
	switch (otherType)
	{
	case ObjectType::Bullet:
	{
		Bullet* bullet = static_cast<Bullet*>(other);
		int damage = bullet->getDamage();
		applyDamage(damage); 
		break;
	}
	case ObjectType::Ammo:
	{
		Ammo* bullet = static_cast<Ammo*>(other);
		int ammo = bullet->getAmmo();
		refillAmmo(ammo);
		break;
	}
	}
}

int Unit::getCurrentHealth() const
{
	return m_currentHealth;
}

int Unit::getMaxHealth() const
{
	return m_maxHealth;
}

float Unit::getPercentHealth() const
{
	return ((float)m_currentHealth / m_maxHealth);
}

void Unit::setFireTarget(const sf::Vector2f & fireTarget)
{
	m_fireTarget = fireTarget;
}

int Unit::getAmmo() const
{
	return m_ammo;
}

Unit::State Unit::getWorldState()
{
	State state;
	state["hiddenCover"] = false;
	state["inCover"] = m_inCover;
	state["hasAmmo"] = m_ammo > 0;
	state["fired"] = false;
	return state;
}

Unit::State Unit::createGoalState()
{
	State state;

	if (m_inCover == false) //if we arent in cover 
	{
		//take cover
		state["hiddenCover"] = true;

		if (getPercentHealth() < 0.2f) //get to cover to heal even if no enemies are around
		{
			int randNum = rand() % 3;
			if (randNum == 0)
				state["healing"] = true;
		}
	}
	else  //in cover
	{
		bool enemiesInRange = getEnemiesInRange(false, m_range).empty() == false; //enemies who are close to us (even if they are on the other side of cover)
		bool enemiesInLOS = getEnemiesInRange(true, m_range * 2, 100).empty() == false;
		if (enemiesInRange)
		{ //if enemies nearby, flee to better cover
			//FleeCover
			state["hiddenCover"] = true;
		}
		if (enemiesInLOS)
		{//if there are enemies within our cover range (can fire twice as far while in cover
			state["hiddenCover"] = true;
			if ( m_reloadTimer > m_reloadTime && //can attack right now
				((m_ammo == 0 && ammoTargetExists()) || m_ammo > 0)) //and either you have ammo or you dont but there is a ammo pickup available
			{
				state["hiddenCover"] = false;
				state["attack"] = true;
			}
		} 
		if (enemiesInRange == false && enemiesInLOS == false && m_inCoverTimer > FLANK_DELAY) //no enemies in sight, in safe place, time to flank!
		{
			state["flankCover"] = true;
			m_inCoverTimer = 0.f;
		}
	}
	return state;
}

void Unit::updatePath()
{
	if (!m_path.empty())
	{
		m_position.x = lerp(m_startPos.x, m_path[m_path.size() - 1].x, m_timer / m_timeToTarget);
		m_position.y = lerp(m_startPos.y, m_path[m_path.size() - 1].y, m_timer / m_timeToTarget);

		if (arrivedAtTarget())
		{
			m_path.pop_back();
			m_startPos = m_position;
			m_timer = 0.f;
			if (m_path.empty() == false)
				m_timeToTarget = (getLength(m_path[m_path.size() - 1] - m_position) / m_moveSpeed);
			else if (m_coverIndexSlot.first != -1)
			{
				m_lastCoverIndexSlot = m_coverIndexSlot;
				m_inCover = true;
				m_inCoverTimer = 0.f;
			}
		}
	}
	else if (m_useHard == false)
	{
		moveTo(m_graph->getRandomIndex());
	}
}


bool Unit::inCover() const
{
	return m_inCover;
}

int Unit::getTeam() const
{
	return m_team;
}

sf::Vector2f Unit::getCoverSlotPosition() const
{
	sf::Vector2f coverSlotPosition(-100, -100);
	if (m_coverIndexSlot.first != -1) //going to cover
	{
		if (m_path.empty() == false)
			coverSlotPosition = m_path[0];
	}
	return coverSlotPosition;
}

std::vector<int> Unit::getEnemiesInRange(bool raycast, int range, int minRange) const
{
	std::vector<int> enemies;
	for (int i = 0; i < m_otherUnits.size(); i++)
	{
		if (m_otherUnits[i].getTeam() != m_team)
		{
			if (m_otherUnits[i].getActive() && (raycast == false || m_graph->raycast(m_position, m_otherUnits[i].getPosition()) == false))
			{
				float l = getLength(m_position - m_otherUnits[i].getPosition());
				if (l < range && l > minRange) //within this range
				{
					enemies.push_back(i);
				}
			}
		}
	}
	return enemies;
}

int Unit::getRange() const
{
	return m_range;
}

bool Unit::arrivedAtTarget() const
{
	if (m_path.empty())
		return false;
	return (getLength(m_path[m_path.size() - 1] - m_position) < 10.f); 
}


bool Unit::getInCover() const
{
	return m_inCover;
}

bool Unit::ammoTargetExists() const
{
	return m_ammoTargetIndex != -1;
}

int Unit::getID() const
{
	return m_id;
}

void Unit::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	GameObject::draw(target, states);
	if (m_active)
		target.draw(m_idText);
}
