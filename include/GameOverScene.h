#pragma once

#include "Scene.h"

class GameOverScene : public Scene {
public:
	GameOverScene(sf::RenderWindow* window) : Scene(window) {};
	void start() override;
	void end() override;
	void pollEvents(sf::Event evt);
	void update(float dt) override;
	void render() const override;

private:
	sf::Text m_outputText;
	sf::Sprite m_background;
};