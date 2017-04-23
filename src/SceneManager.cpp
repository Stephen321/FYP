#include "SceneManager.h"

#include "MenuScene.h"
#include "GameScene.h"
#include "GameOverScene.h"

SceneManager::SceneManager()
	: m_running(true)
{
}

SceneManager & SceneManager::Instance()
{
	static SceneManager sM;
	return sM;
}

SceneManager::~SceneManager()
{
	for (int i = 0; i < m_runningScenes.size(); i++)
	{
		m_runningScenes[i]->end();
		delete m_runningScenes[i];
		m_runningScenes[i] = nullptr;
	}
	m_runningScenes.clear();
}

void SceneManager::init(sf::RenderWindow* window)
{
	m_window = window;
}

void SceneManager::pollEvents()
{
	if (m_runningScenesCount == 0)
		return;
	sf::Event evt;
	while (m_window->pollEvent(evt))
	{
		if ((evt.type == sf::Event::Closed) || (evt.type == sf::Event::KeyReleased) && (evt.key.code == sf::Keyboard::Escape))
		{
			m_running = false;
		}
		m_runningScenes.back()->pollEvents(evt);
	}
}

void SceneManager::update(float dt)
{
	if (m_runningScenesCount == 0)
		return;

	if (dt < 0.2f)
		m_runningScenes.back()->update(dt);
}

void SceneManager::render() const
{
	for (int i = 0; i < m_runningScenes.size(); i++)
	{
		m_runningScenes[i]->render();
	}
}

//push new scene on to the top of stack
void SceneManager::push(SceneType type)
{
	m_runningScenesCount++;
	m_runningScenes.push_back(typeToScene(type));
	m_runningScenes.back()->start();
}

//replace scene at top of stack with new scene
void SceneManager::pop(SceneType type)
{
	if (m_runningScenesCount == 0)
		return;
	m_runningScenesCount--;
	m_runningScenes.back()->end();
	delete m_runningScenes.back();
	m_runningScenes.back() = nullptr;
	m_runningScenes.pop_back();
	push(type);
}

bool SceneManager::getRunning() const
{
	return m_running;
}

void SceneManager::setRunning(bool running)
{
	m_running = running;
}

Scene * SceneManager::typeToScene(SceneType type)
{
	Scene * scene = nullptr;
	switch (type)
	{
	case SceneType::Menu:
		scene = new MenuScene(m_window);
		break;
	case SceneType::Game:
		scene = new GameScene(m_window);
		break;
	case SceneType::GameOver:
		scene = new GameOverScene(m_window);
		break;
	}
	return scene;
}
