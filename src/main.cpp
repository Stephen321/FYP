//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h"
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

 
#include <vector>
#include "SFML\Graphics.hpp"
#include "SceneManager.h"
#include "GameLoader.h"

////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 
int main()
{
	srand((unsigned int)time(NULL));
	sf::RenderWindow window(sf::VideoMode(1366u, 768u, 32), "FYP");
	//	window.setKeyRepeatEnabled(false);
	sf::Clock clock;
	GameLoader loader("assets/"); //loader has to be done before sceneManager creation as sceneManager requires stuff to be 
	//created already (graph for the collision manager to attach to)
	SceneManager& sceneManager = SceneManager::Instance();
	sceneManager.init(&window);
	sceneManager.push(SceneType::Menu);
	while (sceneManager.getRunning())
	{
		sceneManager.pollEvents();
		sceneManager.update(clock.restart().asSeconds());
		sceneManager.render();
	}
	
	return EXIT_SUCCESS;
}