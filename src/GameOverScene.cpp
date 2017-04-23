#include "GameOverScene.h"
#include "SceneManager.h"

#include "GameData.h"

#include "Constants.h"

using namespace Constants;

void GameOverScene::start()
{
	GameData& data = GameData::Instance();

	m_outputText.setFillColor(sf::Color::White);
	m_outputText.setPosition(50, 50);// m_window->getSize().x * 0.5f, m_window->getSize().y * 0.5f);
	std::string str;
	str.append("Team A: (" + data.aDiff + ")\n");
	str.append("\tDeaths: " + std::to_string(data.teamADeaths) + "\n");
	str.append("\tKills: " + std::to_string(data.teamBDeaths) + "\n");
	str.append("\n\n");
	str.append("Team B: (" + data.bDiff + ")\n");
	str.append("\tDeaths: " + std::to_string(data.teamBDeaths) + "\n");
	str.append("\tKills: " + std::to_string(data.teamADeaths) + "\n");
	m_outputText.setString(str);
	m_outputText.setOrigin(m_outputText.getGlobalBounds().width * 0.5f, m_outputText.getGlobalBounds().height * 0.5f);
	m_outputText.setFont(data.font);

	m_background.setTexture(data.gameOverTexture);
}

void GameOverScene::end()
{
}

void GameOverScene::pollEvents(sf::Event evt)
{
	if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Space)
	{
		SceneManager::Instance().pop(SceneType::Menu);
	}
}

void GameOverScene::update(float dt)
{
}

void GameOverScene::render() const
{
	m_window->clear(sf::Color(0, 0, 0));
	m_window->draw(m_background);
	m_window->draw(m_outputText);
	m_window->display();
}
