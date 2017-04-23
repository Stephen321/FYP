#include "MenuScene.h"
#include "SceneManager.h"

#include "GameData.h"

#include "Constants.h"

using namespace Constants;

void MenuScene::start()
{
	GameData& data = GameData::Instance();

	m_outputText.setFillColor(sf::Color::White);
	m_outputText.setPosition(400, 250);
	std::string option1 = DIFF_EASY + " vs " + DIFF_EASY;
	std::string option2 = DIFF_EASY + " vs " + DIFF_HARD;
	std::string option3 = DIFF_HARD + " vs " + DIFF_HARD;
	std::string str;
	str.append("Press 1 to start with " + option1 + "\n\n");
	str.append("Press 2 to start with " + option2 + "\n\n");
	str.append("Press 3 to start with " + option3 + "\n\n");
	m_outputText.setString(str);
	m_outputText.setFont(data.font);

	m_background.setTexture(data.menuTexture);
}

void MenuScene::end()
{
}

void MenuScene::pollEvents(sf::Event evt)
{
	if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Num1)
	{
		GameData& data = GameData::Instance();
		data.aDiff = DIFF_EASY;
		data.bDiff = DIFF_EASY;
		SceneManager::Instance().pop(SceneType::Game);
	}
	else if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Num2)
	{
		GameData& data = GameData::Instance();
		data.aDiff = DIFF_EASY;
		data.bDiff = DIFF_HARD;
		SceneManager::Instance().pop(SceneType::Game);
	}
	else if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Num3)
	{
		GameData& data = GameData::Instance();
		data.aDiff = DIFF_HARD;
		data.bDiff = DIFF_HARD;
		SceneManager::Instance().pop(SceneType::Game);
	}
}

void MenuScene::update(float dt)
{
}

void MenuScene::render() const
{
	m_window->clear(sf::Color(0, 0, 0));
	m_window->draw(m_background);
	m_window->draw(m_outputText);
	m_window->display();
}
