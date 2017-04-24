#include "GameScene.h"
#include "SceneManager.h"
#include "GameData.h"
#include "Constants.h"
using namespace Constants;

GameScene::GameScene(sf::RenderWindow * window)
	: Scene(window)
	, m_map(GameData::Instance().map)
	, m_hud(m_units)
{
	//as long as Graph has been created for the map and added to it
	//this works fine
	//this is ensured when loader is called before scene manager creation
	m_map.addCollisionManager(&m_collisionManager);
}

GameScene::~GameScene()
{
	//for (int i = 0; i < m_units.size(); i++)
	//{
	//	delete m_units[i];
	//	m_units[i] = nullptr;
	//}
}

void GameScene::start()
{
	GameData& data = GameData::Instance();

	m_gameTimer = 0.f;
	data.teamADeaths = 0;
	data.teamBDeaths = 0;
	data.teamAShots = 0;
	data.teamBShots = 0;
	data.teamBNodes = 0;
	data.teamBNodes = 0;

	m_camera.init(m_window->getSize().x, m_window->getSize().y, data.cameraMoveSpeed);
	m_bulletPool = BulletPool(data.bulletPoolSize, data.bulletTexture);
	m_ammoPool = AmmoPool(data.ammoPoolSize, data.ammoPoolTexture, m_map.getGraph());

	int maxHealth = 100;

	//TEAM A
	sf::Vector2f aSpawn = sf::Vector2f(360.f, 550.f);
	m_units.push_back(Unit(aSpawn, data.unitTexture, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_A, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);
	m_units.push_back(Unit(aSpawn + sf::Vector2f(0.f, TILE_SIZE), data.unitTexture, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_A, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);
	m_units.push_back(Unit(aSpawn + sf::Vector2f(0.f, 2 * TILE_SIZE), data.unitTexture, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_A, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);
	m_units.push_back(Unit(aSpawn + sf::Vector2f(0.f, 3 * TILE_SIZE), data.unitTexture, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_A, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);


	//TEAM B
	sf::Vector2f bSpawn = sf::Vector2f(3170.f, 1920.f);
	m_units.push_back(Unit(bSpawn, data.unitTexture2, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_B, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);
	m_units.push_back(Unit(bSpawn + sf::Vector2f(0.f, TILE_SIZE), data.unitTexture2, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_B, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);
	m_units.push_back(Unit(bSpawn + sf::Vector2f(0.f, -2 * TILE_SIZE), data.unitTexture2, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_B, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);
	m_units.push_back(Unit(bSpawn + sf::Vector2f(0.f, -3 * TILE_SIZE), data.unitTexture2, m_map.getGraph(), &m_bulletPool, &m_ammoPool, m_units, data.font));
	m_units.back().init(TEAM_B, data.unitMoveSpeed, data.pistolReloadTime, maxHealth, data.pistolAmmo, data.pistolRange);


	m_map.reset();

	//add collidables
	for (int i = 0; i < m_units.size(); i++)
	{
		m_collisionManager.addCollidable(&m_units[i]);
	}

	for (int i = 0; i < m_map.getCollidables().size(); i++)
	{
		m_collisionManager.addCollidable(&m_map.getCollidables()[i]);
	}
	for (int i = 0; i < m_bulletPool.getBullets().size(); i++)
	{
		m_collisionManager.addCollidable(&m_bulletPool.getBullets()[i]);
	}
	for (int i = 0; i < m_ammoPool.getAmmo().size(); i++)
	{
		m_collisionManager.addCollidable(&m_ammoPool.getAmmo()[i]);
	}

	m_hud.init(m_window->getSize().x, data.portraitTextureA, data.portraitTextureB, data.deadTexture, data.bulletsTexture, data.coverTexture, data.font);
}

void GameScene::end()
{
}

void GameScene::pollEvents(sf::Event evt)
{
	GameData& data = GameData::Instance();
	if (data.debug && evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Space)
	{
		SceneManager::Instance().pop(SceneType::GameOver);
	}
	if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::P)
	{
		data.debug = !data.debug;
	}
	if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Dash)
	{
		data.timeMultiplier -= 0.25f;
		if (data.timeMultiplier <= 1.f)
			data.timeMultiplier = 1.f;
	}
	if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Equal)
	{
		data.timeMultiplier += 0.25f;
	}
	m_camera.pollEvents(evt);
}

void GameScene::update(float dt)
{
	m_camera.update(dt);
	m_bulletPool.update(dt);
	m_ammoPool.update(dt);
	for (int i = 0; i < m_units.size(); i++)
	{
		m_units[i].update(dt);
	}
	m_collisionManager.update();
	m_hud.update(dt);


	m_gameTimer += dt;

	GameData& data = GameData::Instance();
	data.gameTime = m_gameTimer;
	if (data.useTimeToEnd)
	{
		if (m_gameTimer > GAME_TIME_TARGET)
			SceneManager::Instance().pop(SceneType::GameOver);

	}
	else
	{
		if (data.teamADeaths >= DEATH_TARGET)
		{
			data.gameTime = m_gameTimer;
			SceneManager::Instance().pop(SceneType::GameOver);
		}
		else if (data.teamBDeaths >= DEATH_TARGET)
		{
			data.gameTime = m_gameTimer;
			SceneManager::Instance().pop(SceneType::GameOver);
		}
	}
}

void GameScene::render() const
{
	m_window->clear(sf::Color(70, 70, 70));

	m_window->setView(m_camera.getView());
	m_window->draw(m_map);
	for (int i = 0; i < m_units.size(); i++)
	{
		m_window->draw(m_units[i]);
	}
	m_window->draw(m_bulletPool);
	m_window->draw(m_ammoPool);
	m_window->draw(m_collisionManager);

	m_window->setView(m_window->getDefaultView());
	m_window->draw(m_hud);

	m_window->display();
}
