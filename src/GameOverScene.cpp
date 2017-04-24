#include "GameOverScene.h"
#include "SceneManager.h"

#include "GameData.h"

#include "Constants.h"
#include <fstream>
#include <iostream>

using namespace Constants;

void GameOverScene::start()
{
	GameData& data = GameData::Instance();

	m_outputText.setFillColor(sf::Color::White);
	m_outputText.setPosition(50, 50);// m_window->getSize().x * 0.5f, m_window->getSize().y * 0.5f);
	std::string str;
	str.append("Game Time: " + std::to_string(data.gameTime) + " seconds. \n\n");
	str.append("Team A: (" + data.aDiff + ")\n");
	str.append("\tKills: " + std::to_string(data.teamBDeaths) + "\n");
	str.append("\tDeaths: " + std::to_string(data.teamADeaths) + "\n");
	str.append("\tShots: " + std::to_string(data.teamAShots) + "\n");
	str.append("\tNodes: " + std::to_string(data.teamANodes) + "\n");
	str.append("\n");
	str.append("Team B: (" + data.bDiff + ")\n");
	str.append("\tKills: " + std::to_string(data.teamADeaths) + "\n");
	str.append("\tDeaths: " + std::to_string(data.teamBDeaths) + "\n");
	str.append("\tShots: " + std::to_string(data.teamBShots) + "\n");
	str.append("\tNodes: " + std::to_string(data.teamBNodes) + "\n");
	str.append("\nPress space to return to menu.");
	m_outputText.setString(str);
	m_outputText.setOrigin(m_outputText.getGlobalBounds().width * 0.5f, m_outputText.getGlobalBounds().height * 0.5f);
	m_outputText.setFont(data.font);

	if (data.testing == false)
		return;


	std::string state = data.aDiff + "," + data.bDiff + "," + (data.useTimeToEnd ? "time" : "deaths");
	std::cout << "Tests: " << data.testCount << " ." << state << std::endl;
	//first test, initialize first
	if (data.testCount == 0)
	{
		data.sumsA.gameTime = 0.f;
		data.sumsA.kills = 0;
		data.sumsA.deaths = 0;
		data.sumsA.shots = 0;
		data.sumsA.nodes = 0;

		data.sumsB.gameTime = 0.f;
		data.sumsB.kills = 0;
		data.sumsB.deaths = 0;
		data.sumsB.shots = 0;
		data.sumsB.nodes = 0;
	}

	data.sumsA.gameTime += data.gameTime;
	data.sumsA.kills += data.teamBDeaths;
	data.sumsA.deaths += data.teamADeaths;
	data.sumsA.shots += data.teamAShots;
	data.sumsA.nodes += data.teamANodes;

	data.sumsB.gameTime += data.gameTime;
	data.sumsB.kills += data.teamADeaths;
	data.sumsB.deaths += data.teamBDeaths;
	data.sumsB.shots += data.teamBShots;
	data.sumsB.nodes += data.teamANodes;

	data.firstTest = false;



	data.testCount++;
	if (data.testCount == TEST_AMOUNT)
	{
		std::ofstream f;
		std::string fileName = "test_" + data.aDiff + data.bDiff;

		if (data.useTimeToEnd)
			fileName += "time";
		else
			fileName += "deaths";

		//get averages
		std::string GameTime = std::to_string(data.sumsA.gameTime / (float)data.testCount);  //take either a or b time
		std::string A_KILLS = std::to_string(data.sumsA.kills / (float)data.testCount);
		std::string A_DEATHS = std::to_string(data.sumsA.deaths / (float)data.testCount);
		std::string A_SHOTS = std::to_string(data.sumsA.shots / (float)data.testCount);
		std::string A_NODES = std::to_string(data.sumsA.nodes / (float)data.testCount);
		std::string B_KILLS = std::to_string(data.sumsB.kills / (float)data.testCount);
		std::string B_DEATHS = std::to_string(data.sumsB.deaths / (float)data.testCount);
		std::string B_SHOTS = std::to_string(data.sumsB.shots / (float)data.testCount);
		std::string B_NODES = std::to_string(data.sumsB.nodes / (float)data.testCount);

		//print to file
		f.open("assets/tests/" + fileName + ".txt");
		f << state << "\n";
		f << "GameTime, A_KILLS, A_DEATHS, A_SHOTS, A_NODES, B_KILLS, B_DEATHS, B_SHOTS, B_NODES\n";
		f << GameTime << "," << A_KILLS<< "," << A_DEATHS<< "," << A_SHOTS<< "," << A_NODES<< "," << B_KILLS<< "," << B_DEATHS<< "," << B_SHOTS<< "," << B_NODES;
		f.flush();
		f.close();

		if (data.aDiff == DIFF_EASY)
		{
			if (data.bDiff == DIFF_EASY)
			{
				data.bDiff = DIFF_HARD;
			}
			else if (data.bDiff == DIFF_HARD)
			{
				data.aDiff = DIFF_HARD;
			}
		}
		else if (data.aDiff == DIFF_HARD)
		{
			data.testing = false;
		}
		data.testCount = 0;
	}

	if (data.testing)
		SceneManager::Instance().pop(SceneType::Menu);
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
