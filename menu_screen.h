#pragma once

#include "game_data.h"
#include <disreality_engine.h>
#include <vector>

class MenuScreen : public dr::Screen
{
private:
	sf::View mMainView{ {0, 0, GameData::GraphicResolution.x, GameData::GraphicResolution.y} };
	sf::Sprite mBackground;
	dr::Panel mMainMenu{ {600.f, 600.f} };

public:
	void init();
	void inputHandler(sf::Keyboard::Key key, bool isPressed);
	void inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
};