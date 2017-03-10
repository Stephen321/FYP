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

#include <iostream> 
#include <vector>
#include "MenuScreen.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

////////////////////////////////////////////////////////////
///Entrypoint of application 
//////////////////////////////////////////////////////////// 
int main()
{
	srand((unsigned int)time(NULL));
	sf::RenderWindow window(sf::VideoMode(1280u, 720u, 32), "FYP");
	std::vector<Screen*> Screens;
	int screen = 0;
	MenuScreen menuScreen;
	Screens.push_back(&menuScreen);
	GameScreen gameScreen;
	Screens.push_back(&gameScreen);
	GameOverScreen gameOverScreen;
	Screens.push_back(&gameOverScreen);

	while (screen >= 0)
	{
		screen = Screens[screen]->run(window);
	}
	return EXIT_SUCCESS;
}