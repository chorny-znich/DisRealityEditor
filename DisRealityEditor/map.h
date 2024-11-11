#pragma once
#include <SFML/Graphics.hpp>

/**
 * @brief A game map
*/
class Map
{
private:
  int mMapIndex{ 0 };
  sf::Vector2i mMapSize{ 3, 3 };

public:
  void setMapIndex(int index);
  int& getMapIndexRef();
  void setMapSize(sf::Vector2i size);
  sf::Vector2i& getMapSizeRef();
};

