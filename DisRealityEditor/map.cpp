#include "map.h"
#include "imgui/imgui.h"
#include <DisRealityGF.h>

void Map::createMap(size_t index)
{
  for (size_t y = 0; y < mMapSize.y; y++) {
    for (size_t x = 0; x < mMapSize.x; x++) {
      Location loc;
      loc.setId(y * mMapSize.x + x);
      loc.setPosition({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) });
      loc.setFloorLayerId("dirt_1");
      /*loc.setObjectLayerId(section.at("objects_layer"));
      loc.setBarrier(std::stoi(section.at("barrier")));*/
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

void Map::update(sf::Time dt)
{
  ImGui::Begin("Map menu");
  ImGui::InputInt("Map index", &(mMapIndex));
  ImGui::InputInt("Map size x", &(mMapSize.y));
  ImGui::InputInt("Map size y", &(mMapSize.x));
  if (ImGui::Button("Create map")) {
    createMap(mMapIndex);
    createFloorMap();
  }
  ImGui::End();
}

