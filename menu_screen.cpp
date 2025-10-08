#include "menu_screen.h"
#include "screen_manager.h"
#include <iostream>
#include <format>
#include "about_screen.h"
#include "editor_screen.h"

void MenuScreen::init()
{
  mBackground.setTexture(dr::Textures::get("menu_screen_background"));
	mMainMenu.setPosition({ 750.f, 400.f });
	mMainMenu.setColor(sf::Color(255, 255, 255, 0));
	
	mMainMenu.addTextButton({ 300.f, 120.f }, dr::TextManager::get("play_button"));
	mMainMenu.addTextButton({ 300.f, 120.f }, dr::TextManager::get("about_button"));
	mMainMenu.addTextButton({ 300.f, 120.f }, dr::TextManager::get("exit_button"));
}

void MenuScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
}

void MenuScreen::inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window)
{
	sf::Vector2f worldCoords = window->mapPixelToCoords(position, mMainView);
	int buttonIndex = mMainMenu.handleInput(worldCoords);

	switch (buttonIndex)
	{
	case 0:
		dr::ScreenManager::createScreen<EditorScreen>("EDITOR");
		break;
	case 1:
		dr::ScreenManager::createScreen<AboutScreen>("ABOUT");
		break;
	case 2:
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
	mMainMenu.render(window);
}
