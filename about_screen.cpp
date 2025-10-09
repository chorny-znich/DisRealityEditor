#include "about_screen.h"

void AboutScreen::init()
{
	sf::Text description = dr::TextManager::get("description");
	description.setPosition(200, 500);
	mInfo.push_back(description);

	sf::Text version = dr::TextManager::get("version");
	version.setPosition(900, 600);
	mInfo.push_back(version);
	
	mMainMenu.setPosition({ 850.f, 900.f });
	mMainMenu.setColor(sf::Color(255, 255, 255, 0));
	mMainMenu.addTextButton({ 300.f, 120.f }, dr::TextManager::get("back_button"));
}

void AboutScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
}

void AboutScreen::inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window)
{
	if (button == sf::Mouse::Left && isPressed) {
		sf::Vector2f worldCoords = window->mapPixelToCoords(position, mMainView);
		int buttonIndex = mMainMenu.handleInput(worldCoords);

		switch (buttonIndex)
		{
		case 0:
			dr::GameState::destroyScreen();
			break;
		}
	}
}

void AboutScreen::update(sf::Time dt)
{
}

void AboutScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
	mMainMenu.render(window);
	for (const auto& txt : mInfo) {
		window.draw(txt);
	}
}