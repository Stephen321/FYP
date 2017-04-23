#pragma once

#include "Scene.h"

class MenuScene : public Scene {
public:
	MenuScene(sf::RenderWindow* window) : Scene(window) {};
	void start() override;
	void end() override;
	void pollEvents(sf::Event evt);
	void update(float dt) override;
	void render() const override;

private:
	sf::Text m_outputText;
	sf::Sprite m_background;
};