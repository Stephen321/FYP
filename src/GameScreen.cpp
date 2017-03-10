#include "GameScreen.h"

int GameScreen::run(sf::RenderWindow &window)
{
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	typedef std::unordered_map<std::string, GameObjectPtrVector> GameObjectMap;
	GameLoader gameLoader("assets/map/");
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	sf::Font font;
	font.loadFromFile("assets/fonts/GROBOLD.ttf");
	sf::Text text("test", font, 30.f);
	text.setFillColor(sf::Color::White);
	text.setPosition(10.f, 10.f);
	text.setStyle(sf::Text::Regular);

	sf::View view = window.getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	
	sf::Vector2f worldSize(bounds.width * Constants::WORLD_SCREEN_SIZES, bounds.height);

	GameObjectMap gameObjectsMap; 
	gameObjectsMap[Constants::MISC_KEY] = GameObjectPtrVector();

	std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player(sf::Vector2f(500.f, worldSize.y * 0.5f), worldSize));
	gameObjectsMap.at(Constants::MISC_KEY).push_back(player);

	//debug
	bool pause = false;
	float zoom = 1.f;
	bool zoomed = false;
	sf::RectangleShape boundsRect;
	boundsRect.setFillColor(sf::Color::Transparent);
	boundsRect.setOutlineColor(sf::Color::Yellow);
	boundsRect.setOutlineThickness(4.f);
	boundsRect.setPosition(bounds.left, bounds.top);
	boundsRect.setSize(sf::Vector2f(bounds.width, bounds.height));
	
	while (Running)
	{
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed || (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape))
			{
				return (-1);
			}

			if (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Return)
			{
				//std::cout << "Going to screen: " << 2 << std::endl;
				//return (2);
			}
			if (Event.type == sf::Event::MouseWheelScrolled)
			{
				if (Event.mouseWheelScroll.delta < 0 && zoom > 1)
					zoom = 1;
				else if (Event.mouseWheelScroll.delta > 0 && zoom < 1)
					zoom = 1;
				zoom += Event.mouseWheelScroll.delta / 100.f;
				zoomed = true;
			}

			if (Event.type == sf::Event::MouseButtonReleased)
			{
				if (Event.mouseButton.button == sf::Mouse::Button::Right)
					view.reset(sf::FloatRect(0.f, 0.f, (float)window.getSize().x, (float)window.getSize().y));
			}
			if (Event.type == sf::Event::LostFocus)
			{
				pause = true;
			}
			if (Event.type == sf::Event::GainedFocus)
			{
				pause = false;
			}				
		}		

		float dt = frameClock.restart().asSeconds();
		if (dt > 0.3f || pause)
			dt = 0.f;
		
		//update camera
		view.setCenter(player->getPosition().x , view.getCenter().y);
		bounds.left = view.getCenter().x - (bounds.width * 0.5f);
		bounds.top = view.getCenter().y - (bounds.height * 0.5f);
		window.setView(view);

		window.clear(sf::Color(96, 23, 54));


		for (GameObjectMap::iterator it = gameObjectsMap.begin(); it != gameObjectsMap.end(); ++it)
		{
			GameObjectPtrVector& v = it->second;
			for (GameObjectPtrVector::iterator itV = v.begin(); itV != v.end();)
			{				
				std::shared_ptr<GameObject>& gameObject = (*itV);
				GameObject::Type type = gameObject->getType();
				//update 
				gameObject->update(dt); //TODO: another loop before .clear
				std::pair<GameObject::Type, sf::Vector2f> entity;
				entity.first = gameObject->getType();
				entity.second = gameObject->getPosition();

				//draw
				if (gameObject->getRect().intersects(bounds)) 
				{
					window.draw(*gameObject);
				}

				//remove if not active
				if (gameObject->getActive() == false)
				{
					itV = v.erase(itV);
					if (v.empty())
					{
						break;
					}
				}
				else
				{
					++itV;
				}
			}
		}

		window.draw(text);
		
		window.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}

sf::FloatRect GameScreen::getRectFromView(const sf::View & view)
{
	return sf::FloatRect(view.getCenter().x - view.getSize().x * 0.5f,
		view.getCenter().y - view.getSize().y * 0.5f,
		view.getSize().x,
		view.getSize().y);
}

void GameScreen::drawGameObject(sf::RenderTarget & target, std::shared_ptr<GameObject>& gameObject, const sf::FloatRect & viewBounds)
{
	target.setView(sf::View(viewBounds));
	if (gameObject->getRect().intersects(getRectFromView(target.getView())))
	{
		target.draw(*gameObject);
	}
	target.setView(target.getDefaultView());
}

void GameScreen::checkForCollisions(std::vector<std::shared_ptr<GameObject>>& v, std::shared_ptr<GameObject>& collider)
{
	typedef std::vector<std::shared_ptr<GameObject>> GameObjectPtrVector;
	GameObjectPtrVector::iterator begin = v.begin();
	GameObjectPtrVector::iterator end = v.end();
	for (GameObjectPtrVector::iterator it = begin; it != end; ++it)
	{
		std::shared_ptr<GameObject> gameObject = (*it);
		gameObject->collision(collider);
	}
}