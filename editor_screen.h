#pragma once
#include <screen.h>
#include "game_data.h"
#include <disreality_engine.h>

class EditorScreen : public dr::Screen
{
private:
	sf::View mMainView{ {0, 0, GameData::GraphicResolution.x, GameData::GraphicResolution.y} };

	enum class State {
		MENU,
		CREATE_MAP,
		LOAD_MAP,
		VIEW,
		EDIT
	};
	State mState{ State::MENU };

	dr::Map mCurrentMap;
	dr::RenderComponent mRenderComponent;
	dr::InputComponent mInputComponent{ mMainView };

	void loadMap(int mapIndex);
public:
	EditorScreen();
	void init();
	void inputHandler(sf::Keyboard::Key key, bool isPressed);
	void inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
};