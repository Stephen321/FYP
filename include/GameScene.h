#pragma once

#include "Scene.h"
#include "Map.h"
#include "Camera.h"
#include "Unit.h"
#include "BulletPool.h"
#include "AmmoPool.h"
#include "CollisionManager.h"
#include "HUD.h"

class GameScene : public Scene {
public:
	GameScene(sf::RenderWindow* window);
	~GameScene();
	void start() override;
	void end() override;
	void pollEvents(sf::Event evt);
	void update(float dt) override;
	void render() const override;
private:
	Camera m_camera;
	BulletPool m_bulletPool;
	AmmoPool m_ammoPool;
	Map& m_map;
	std::vector<Unit> m_units;
	CollisionManager m_collisionManager;
	HUD m_hud;
	float m_gameTimer;
	const int DEATH_TARGET = 25; // deaths to end game
	const int GAME_TIME_TARGET = 600; //10 minutes 
};