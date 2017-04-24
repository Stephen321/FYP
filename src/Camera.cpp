#include "Camera.h"

#include "GameData.h"

Camera::Camera()
	: m_zoom(1.f)
	, m_zooming(false)
{
}

void Camera::init(float windowWidth, float windowHeight, int moveSpeed)
{
	m_view.reset(sf::FloatRect(0.f, 0.f, windowWidth, windowHeight));
	m_view.zoom(2.2f);
	m_view.setCenter(m_view.getSize().x * 0.5f + 150, m_view.getSize().y * 0.5f);
	m_moveSpeed = moveSpeed;
}

sf::View Camera::getView() const
{
	return m_view;
}

void Camera::update(float dt)
{
	dt /= GameData::Instance().timeMultiplier;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_view.move(0.f, -m_moveSpeed * dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_view.move(0.f, m_moveSpeed * dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_view.move(-m_moveSpeed * dt, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_view.move(m_moveSpeed * dt, 0.f);
	}

	if (m_zooming)
	{
		m_view.zoom(m_zoom);
		m_zooming = false;
	}
}

void Camera::pollEvents(sf::Event evt)
{
	if (evt.type == sf::Event::MouseWheelScrolled)
	{
		if (evt.mouseWheelScroll.delta < 0 && m_zoom > 1.f)
			m_zoom = 1.f;
		else if (evt.mouseWheelScroll.delta > 0 && m_zoom < 1.f)
			m_zoom = 1.f;
		m_zoom -= evt.mouseWheelScroll.delta / 100.f;
		m_zooming = true;
	}
}
