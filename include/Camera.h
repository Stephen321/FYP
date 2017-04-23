#pragma once

#include "SFML\Graphics.hpp"

class Camera
{
public:
	Camera();
	void init(float windowWidth, float windowHeight, int moveSpeed);
	sf::View getView() const;
	void update(float dt);
	void pollEvents(sf::Event evt);
private:
	sf::View m_view;
	float m_zoom;
	bool m_zooming;
	int m_moveSpeed;
};