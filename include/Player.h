#pragma once

#include <memory>
#include "GameObject.h"
#include "HealthBar.h"
#include "Constants.h"

class Player : public GameObject
{
public:
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	Player(const sf::Vector2f& startPos, const sf::Vector2f& worldSize);
	void update(float dt) override;
	bool BombAvailable() const;
	void smartBomb();
	bool canHyperJump() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool collision(const std::shared_ptr<GameObject>& collidor);
	int getScore();
	void increaseScore(int value);

private:
	void checkInput();

	//lasers
	float m_liveTimer;
	void fire(); 
	void hyperJump();
	void replenishHyperJump();
	bool m_canHyperJump;
	bool m_bombReady;
	const float SMARTBOMB_COOLDOWN = 60.f;
	const float RELOAD_TIME = 0.1f;
	float m_smartBombTimer = 0;
	float m_reloadTimer;
	const float ANGLE_OFFSET = 180.f;
	HealthBar m_healthBar;
	const float HEALTH_Y_OFFSET = 30.f;

	int m_score;
	const float DAMAGE = 10.f;
};