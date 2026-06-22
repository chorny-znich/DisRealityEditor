#pragma once
#include "game_data.h"
#include <disreality_engine.h>

struct ScreenInputVisitor;

/**
 * @brief Information about the application
 */
class AboutScreen : public dr::Screen
{ 
public:
	void init() override;
	void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
private:
	struct ScreenInputVisitor;

	sf::View mMainView{ sf::FloatRect({0, 0}, {gd::GraphicsResolution.x, gd::GraphicsResolution.y}) };
	sf::Sprite mBackground{ dr::Textures::get("background") };
	std::vector<sf::Text> mInfo;
	dr::TextButton mBackButton{ {150.f, 50.f}, dr::TextManager::get("normal_button")};
};