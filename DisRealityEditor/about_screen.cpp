#include "about_screen.h"

void AboutScreen::init()
{
  sf::Text aboutInfo = dr::TextManager::get("AboutText_1");
	aboutInfo.setPosition(200, 500);
  mInfo.push_back(aboutInfo);

  dr::Button buttonBack( { 200, 75 }, dr::TextManager::get("BackButton"));
	buttonBack.setPosition({ 900, 1000 });
  mButtons.push_back(buttonBack);
}

void AboutScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
}

void AboutScreen::inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window)
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
		dr::GameState::destroyScreen();
		break;
	}
}

void AboutScreen::update(sf::Time dt)
{
}

void AboutScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);

	for (auto& txt : mInfo) {
		window.draw(txt);
	}
	for (auto& btn : mButtons) {
		btn.render(window);
	}
}