// The game map
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "location.h"
#include <DisRealityGF.h>

class Map
{
private:
  const sf::Vector2f mTileSize{ 128, 128 };
  sf::Vector2u mMapSize;
  sf::VertexArray mFloorMap;
  std::vector<Location> mLocations;

public:
  void createMap(size_t index);
  void createFloorMap();
  const sf::VertexArray& getFloorMap() const;
  std::vector<sf::Sprite> getStaticActors() const;
};