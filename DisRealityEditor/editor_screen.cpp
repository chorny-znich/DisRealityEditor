#include "editor_screen.h"
#include "imgui/imgui.h"

void EditorScreen::init()
{
}

void EditorScreen::inputHandler(sf::Vector2i position, sf::RenderWindow* window)
{
}

void EditorScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
}

void EditorScreen::inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window)
{
}

void EditorScreen::update(sf::Time dt)
{
  ImGui::Begin("Editor menu");
  ImGui::End();
}

void EditorScreen::render(sf::RenderWindow& window)
{
}
