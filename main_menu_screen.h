#pragma once
#include <disreality_engine.h>
#include "game_data.h"
#include <vector> 

/**
 * @brief Mainmenu of the app
 */
class MainMenuScreen : public dr::Screen
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
  std::vector<dr::TextButton> mMenuButtons;
};