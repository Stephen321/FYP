#pragma once

#include <vector>
#include "SceneType.h"
#include "Scene.h"

class SceneManager {
public:
	static SceneManager& Instance();
	~SceneManager();
	void init(sf::RenderWindow* window);
	void pollEvents();
	void update(float dt);
	void render() const;
	void push(SceneType type);
	void pop(SceneType type);
	bool getRunning() const;
	void setRunning(bool running);

	SceneManager(SceneManager const&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager &&) = delete;  
private:
	SceneManager();
	bool m_running;
	sf::RenderWindow* m_window;
	Scene * typeToScene(SceneType type);
	std::vector<Scene*> m_runningScenes;
	int m_runningScenesCount;
};