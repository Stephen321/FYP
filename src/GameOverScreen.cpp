#include "GameOverScreen.h"

int GameOverScreen::run(sf::RenderWindow &window)
{
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	while (Running)
	{
		float dt = frameClock.restart().asSeconds();

		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}

			if (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Space)
			{
				return (0);
			}
		}

		window.clear();

		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}