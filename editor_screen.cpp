#include "editor_screen.h"
#include <imgui.h>
#include <imgui-SFML.h>

#include "about_screen.h"

/**
 * @brief For handling Events in handleInput method
 */
struct AboutScreen::ScreenInputVisitor
{
	AboutScreen& screen;
	sf::RenderWindow& window;

	void clearUI()
	{
		window.setMouseCursor(dr::CursorManager::get("arrow"));
		screen.mBackButton.clearOverlap();
	}

	void operator()(const sf::Event::MouseMoved& mouseMoved)
	{
		sf::Vector2f mouseViewCoords = window.mapPixelToCoords(mouseMoved.position);
		if (screen.mBackButton.isOverlap(mouseViewCoords))
		{
			window.setMouseCursor(dr::CursorManager::get("hand"));
		}
		else
		{
			window.setMouseCursor(dr::CursorManager::get("arrow"));
		}
	}

	/**
	 * @brief Mouse button clicked event
	 * @param
	 */
	void operator()(const sf::Event::MouseButtonPressed mouseButton)
	{
		if (mouseButton.button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mouseViewCoords = window.mapPixelToCoords(mouseButton.position);
			if (screen.mBackButton.isClicked(mouseViewCoords))
			{
				clearUI();
				dr::ScreenManager::destroyScreen();
				window.setMouseCursor(dr::CursorManager::get("arrow"));
			}
		}
	}

	void operator()(const auto&) {};
};

/**
 * @brief Initialize screen resources
 */
void AboutScreen::init()
{
	ImGui::SFML::Init()
}

void AboutScreen::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
	event.visit(ScreenInputVisitor{ *this, window });
}

/**
 * @brief 
 * @param dt 
 */
void AboutScreen::update(float dt)
{
	ImGui::SFML::Update(dt);
}

/**
 * @brief 
 * @param window 
 */
void AboutScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
	ImGui::SFML::Render(window);
}
