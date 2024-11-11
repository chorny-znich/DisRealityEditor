#include "map.h"

void Map::setMapIndex(int index)
{
  mMapIndex = index;
}

int& Map::getMapIndexRef()
{
  return mMapIndex;
}

void Map::setMapSize(sf::Vector2i size)
{
  mMapSize = size;
}

sf::Vector2i& Map::getMapSizeRef()
{
  return mMapSize;
}
