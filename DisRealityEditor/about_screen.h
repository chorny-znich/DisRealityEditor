#pragma once

#include "game_data.h"
#include <DisRealityGF.h>

class AboutScreen : public dr::Screen
{
private:
	sf::View mMainView{ {0, 0, GameData::GraphicResolution.x, GameData::GraphicResolution.y} };
	std::vector<sf::Text> mInfo;
	std::vector<dr::Button> mButtons;
public:
	void init();
	void inputHandler(sf::Vector2i position, sf::RenderWindow* window);
	void inputHandler(sf::Keyboard::Key key, bool isPressed);
	void inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
};

