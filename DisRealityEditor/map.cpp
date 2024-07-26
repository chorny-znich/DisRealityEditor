#include "map.h"
#include <iostream>
#include <format>
#include <fstream>

void Map::createMap(size_t index)
{
  ini::Document doc = ini::load("data/map/map_0.ini");
  ini::Section section = doc.getSection("map_size");
  mMapSize.x = std::stoul(section.at("width"));
  mMapSize.y = std::stoul(section.at("height"));

  for (size_t y = 0; y < mMapSize.y; y++) {
    for (size_t x = 0; x < mMapSize.x; x++) {
      ini::Section section = doc.getSection("loc_" + std::to_string(y) + "_" + std::to_string(x));
      Location loc;
      loc.setId(y * mMapSize.x + x);
      loc.setPosition({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) });
      loc.setFloorLayerId(section.at("floor_layer"));
      loc.setObjectLayerId(section.at("objects_layer"));
      loc.setBarrier(std::stoi(section.at("barrier")));
      mLocations.push_back(std::move(loc));
    }
  }
}

void Map::createFloorMap()
{
  const int VERTEX_IN_QUAD = 4;

  int vertexCounter = 0;
  mFloorMap.setPrimitiveType(sf::Quads);
  mFloorMap.resize(mMapSize.x * mMapSize.y * VERTEX_IN_QUAD);

  for (size_t y{ 0 }; y < mMapSize.y; y++) {
    for (size_t x{ 0 }; x < mMapSize.x; x++) {
      mFloorMap[vertexCounter + 0].position = sf::Vector2f(x * mTileSize.x, y * mTileSize.y);
      mFloorMap[vertexCounter + 1].position = sf::Vector2f(x * mTileSize.x + mTileSize.y, 
        y * mTileSize.y);
      mFloorMap[vertexCounter + 2].position = sf::Vector2f(x * mTileSize.x + mTileSize.x, 
        y * mTileSize.y + mTileSize.y);
      mFloorMap[vertexCounter + 3].position = sf::Vector2f(x * mTileSize.x, 
        y * mTileSize.y + mTileSize.y);

      const std::string id = mLocations.at(y * mMapSize.x + x).getFloorLayerId();
      sf::Vector2f textCoord = dr::Database::getSprite(id);
      mFloorMap[vertexCounter + 0].texCoords = sf::Vector2f(textCoord.x, textCoord.y);
      mFloorMap[vertexCounter + 1].texCoords = sf::Vector2f(textCoord.x + mTileSize.x, 
        textCoord.y);
      mFloorMap[vertexCounter + 2].texCoords = sf::Vector2f(textCoord.x + mTileSize.x, 
        textCoord.y + mTileSize.y);
      mFloorMap[vertexCounter + 3].texCoords = sf::Vector2f(textCoord.x, 
        textCoord.y + mTileSize.y);

      vertexCounter += VERTEX_IN_QUAD;
    }
  }
}

const sf::VertexArray& Map::getFloorMap() const
{
  return mFloorMap;
}

std::vector<sf::Sprite> Map::getStaticActors() const
{
  std::vector<sf::Sprite> result;
  for (const auto loc : mLocations) {
    if (loc.getObjectLayerId() != "none") {
      dr::Tile tile = dr::Database::getTile(loc.getObjectLayerId());
      sf::Sprite sprite;
      sprite.setTexture(dr::Textures::get(tile.mTextureId));
      sprite.setTextureRect({static_cast<int>(dr::Database::getSprite(tile.mSpriteId).x), 
        static_cast<int>(dr::Database::getSprite(tile.mSpriteId).y), static_cast<int>(mTileSize.x),
        static_cast<int>(mTileSize.y) });
      sprite.setPosition({ loc.getPosition().x * mTileSize.x, loc.getPosition().y * mTileSize.y });
      result.push_back(std::move(sprite));
    }
  }

  return result;
}
