#include "render.h"
#include <DisRealityGF.h>

void Render::initFloorLayer(const sf::VertexArray& arr)
{
  mFloorLayer = arr;
}

void Render::addStaticActor(sf::Sprite& sprite)
{
  mStaticActor.push_back(sprite);
}

void Render::addDynamicActor(sf::Sprite& sprite)
{
  mDynamicActor.push_back(sprite);
}

void Render::render(sf::RenderWindow& window)
{
  using Textures = dr::AssetManager<std::string, sf::Texture>;

  window.clear();
  window.draw(mFloorLayer, &(Textures::get("floor_texture")));
  for (const auto& sa : mStaticActor) {
    window.draw(sa);
  }
  for (const auto& da : mDynamicActor) {
    window.draw(da);
  }
  window.display();
}
