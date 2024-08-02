#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @brief Component for the graphics rendering
*/
class Render
{
private:
  sf::VertexArray mFloorLayer;
  std::vector<sf::Sprite> mStaticActor;
  std::vector<sf::Sprite> mDynamicActor;
public:
  void initFloorLayer(const sf::VertexArray& arr);
  void addStaticActor(sf::Sprite& sprite);
  void addDynamicActor(sf::Sprite& sprite);
  void render(sf::RenderWindow& window);
};