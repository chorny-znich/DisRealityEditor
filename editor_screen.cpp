#include "editor_screen.h"
#include <imgui.h>
#include <imgui-SFML.h>

#include "about_screen.h"

/**
 * @brief For handling Events in handleInput method
 */
struct EditorScreen::ScreenInputVisitor
{
	EditorScreen& screen;
	sf::RenderWindow& window;

	void clearUI()
	{
		window.setMouseCursor(dr::CursorManager::get("arrow"));
	}

	void operator()(const sf::Event::MouseMoved& mouseMoved)
	{
		sf::Vector2f mouseViewCoords = window.mapPixelToCoords(mouseMoved.position);
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
		}
	}

	void operator()(const auto&) {};
};

/**
 * @brief Initialize screen resources
 */
void EditorScreen::init()
{
	ImGui::SFML::Init(dr::ImguiHelper::getWindow());
}

void EditorScreen::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
	event.visit(ScreenInputVisitor{ *this, window });
}

/**
 * @brief 
 * @param dt 
 */
void EditorScreen::update(float dt)
{
	ImGui::SFML::Update(dr::ImguiHelper::getWindow(), dr::ImguiHelper::getTime());
}

/**
 * @brief 
 * @param window 
 */
void EditorScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
	ImGui::SFML::Render(window);
}
