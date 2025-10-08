#pragma once

#include "game_data.h"
#include <disreality_engine.h>

class AboutScreen : public dr::Screen
{
private:
	sf::View mMainView{ {0, 0, GameData::GraphicResolution.x, GameData::GraphicResolution.y} };
	dr::Panel mMainMenu{ {350.f, 150.f} };
	std::vector<sf::Text> mInfo;
public:
	void init();
	void inputHandler(sf::Keyboard::Key key, bool isPressed);
	void inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
};