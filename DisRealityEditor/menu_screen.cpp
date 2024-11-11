#include "menu_screen.h"
#include "screen_manager.h"
#include <iostream>
#include <format>

void MenuScreen::init()
{
  mBackground.setTexture(dr::Textures::get("menu_screen_background"));
	
  dr::Button buttonAbout( { 200, 75 }, dr::TextManager::get("about_button"));
	buttonAbout.setPosition({ 900, 800 });
  mButtons.push_back(buttonAbout);
  dr::Button buttonExit( { 200, 75 }, dr::TextManager::get("exit_button"));
	buttonExit.setPosition( { 900, 900 } );
  mButtons.push_back(buttonExit);
}

void MenuScreen::inputHandler(sf::Vector2i position, sf::RenderWindow* window)
{
}

void MenuScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
}

void MenuScreen::inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window)
{
	size_t buttonIndex = -1;

	for (size_t i = 0; i < mButtons.size(); ++i)
	{
		sf::Vector2f worldCoords = window->mapPixelToCoords(position, mMainView);
		if (mButtons[i].isClicked({ worldCoords.x, worldCoords.y }))
		{
			buttonIndex = i;
			break;
		}
	}

	switch (buttonIndex)
	{
	case 0:
		ScreenManager::createScreen(GameData::Screens::ABOUT);
		break;
	case 1:
		dr::GameState::destroyScreen();
		break;
	}
}

void MenuScreen::update(sf::Time dt)
{
}

void MenuScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
  window.draw(mBackground);
	for (auto& btn : mButtons) {
    btn.render(window);
  }
}
