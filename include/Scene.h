#pragma once


#include "SFML\Graphics.hpp"

class Scene {
public:
	Scene(sf::RenderWindow* window) : m_window(window) {}
	virtual void start() = 0;
	virtual void end() = 0;
	virtual void pollEvents(sf::Event evt) = 0;
	virtual void update(float dt) = 0;
	virtual void render() const =0;
protected:
	sf::RenderWindow* m_window;
};