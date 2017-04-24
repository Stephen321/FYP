#pragma once

#include "GameObject.h"
#include "BulletPool.h"
#include "AmmoPool.h"
#include "GoapAgent.h"

class Unit : public GameObject, public GoapAgent{
public:
	typedef std::unordered_map<std::string, bool> State;

	Unit() : GameObject(), m_bulletPool(), m_otherUnits(std::vector<Unit>()) {};
	Unit(const sf::Vector2f& spawnPos, const sf::Texture& texture, Graph* graph, BulletPool* bulletPool, AmmoPool* ammoPool, std::vector<Unit>& otherUnits, const sf::Font& font);
	void init(int team, float moveSpeed, float reloadTime, int maxHealth, int maxAmmo, int range);
	void update(float dt) override;
	void moveToAmmo();
	void moveTo(int goal);
	void moveToFlankCover();
	void moveToCover(const std::string& coverFromDirection = "");
	void moveFromTo(int start, int goal);

	void fireAtTarget(bool fromCover);
	void collision(ObjectType otherType, Collidable* other) override;
	int getCurrentHealth() const;
	int getMaxHealth() const;
	float getPercentHealth() const;
	void setFireTarget(const sf::Vector2f& fireTarget);
	int getAmmo() const;

	//implement state creation and world data and plan events 
	State getWorldState();
	State createGoalState();

	void updatePath();

	bool inCover() const;
	int getTeam() const;
	sf::Vector2f getCoverSlotPosition() const;
	std::vector<int> getEnemiesInRange(bool raycast, int range = 400, int minRange = 0) const;
	int getRange() const;
	bool arrivedAtTarget() const;
	bool getInCover() const;
	bool ammoTargetExists() const;
	int getID() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:

	void setPath(const std::vector<sf::Vector2f> path);
	void applyDamage(int damage);
	void refillAmmo(int ammo);
	void respawn(float dt);
	void reset();
	void removeFromCover();
	bool m_useHard;
	int m_team;
	int m_range;
	std::vector<Unit>& m_otherUnits;
	int m_lastAmmoTargetIndex;
	int m_ammoTargetIndex;
	sf::Text m_idText;
	static int ID;
	int m_id;


	// could fire off events which are handled by something that then fires/gets path for unit
	Graph* m_graph;
	BulletPool* m_bulletPool;
	AmmoPool* m_ammoPool;
	sf::Vector2f m_coverSlotPosition;
	std::vector<sf::Vector2f> m_path;
	float m_timer;
	sf::Vector2f m_startPos;
	sf::Vector2f m_fireTarget;
	sf::Vector2f m_spawnPos;
	float m_faceDirection;
	float m_timeToTarget;
	float m_moveSpeed;
	float m_reloadTime;
	float m_reloadTimer;
	int m_ammo;
	int m_maxAmmo;
	int m_currentHealth;
	int m_maxHealth;
	float m_respawnTimer;
	float m_healthTimer;
	
	float m_inCoverTimer;
	const int FLANK_DELAY = 5;

	static const int HEALTH_RECOVER_TIME = 3;
	static const int HEALTH_RECOVER_AMOUNT = 2;
	static const int CHOOSE_COVER = 3;

	bool m_inCover;
	std::pair<int, std::string> m_coverIndexSlot;
	std::pair<int, std::string> m_lastCoverIndexSlot;
};