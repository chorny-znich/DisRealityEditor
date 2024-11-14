#pragma once
#include <screen.h>
#include "render_component.h"
#include "map.h"

class EditorScreen : public dr::Screen
{
private:
	sf::View mMainView{ {0, 0, GameData::GraphicResolution.x, GameData::GraphicResolution.y} };
	RenderComponent mRenderComponent;
	Map mCurrentMap;

public:
	void init();
	void inputHandler(sf::Vector2i position, sf::RenderWindow* window);
	void inputHandler(sf::Keyboard::Key key, bool isPressed);
	void inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
};