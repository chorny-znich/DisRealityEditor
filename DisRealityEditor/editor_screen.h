#pragma once
#include <screen.h>

class EditorScreen : public dr::Screen
{
public:
	void init();
	void inputHandler(sf::Vector2i position, sf::RenderWindow* window);
	void inputHandler(sf::Keyboard::Key key, bool isPressed);
	void inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window);
	void update(sf::Time dt);
	void render(sf::RenderWindow& window);
};