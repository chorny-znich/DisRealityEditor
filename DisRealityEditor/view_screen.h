#pragma once
#include <DisRealityGF.h>
#include "game_data.h"
#include "render.h"
#include "map.h"

/**
 * @brief View game's map
*/
class ViewScreen : public dr::Screen
{
private:
	sf::View mMainView{ {0, 0, GameData::GraphicResolution.x, GameData::GraphicResolution.y} };
	Render mRender;
	Map mMap;
public:
	void init();
	void inputHandler(sf::Keyboard::Key key, bool isPressed);
	void inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
};

