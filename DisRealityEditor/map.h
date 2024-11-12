#pragma once
#include "location.h"
#include <SFML/Graphics.hpp>

/**
 * @brief A game map
*/
class Map
{
private:
  int mMapIndex{ 0 };
  sf::Vector2i mMapSize{ 3, 3 };
  sf::VertexArray mFloorMap;
  std::vector<Location> mLocations;

public:
  void createMap(size_t index);
  void createFloorMap();
  const sf::VertexArray& getFloorMap() const;
  void update(sf::Time dt);
};