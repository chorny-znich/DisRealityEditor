#pragma once
#include <disreality_engine.h>

/**
 * @brief 
 */
class EditorScreen : public dr::Screen
{
public:
	void init() override;
	void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
private:
	struct ScreenInputVisitor;

	sf::View mMainView{ sf::FloatRect({0, 0}, {gd::GraphicsResolution.x, gd::GraphicsResolution.y}) };
};