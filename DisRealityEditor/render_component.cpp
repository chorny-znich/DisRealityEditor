#include "render_component.h"
#include <DisRealityGF.h>

void RenderComponent::initFloorLayer(const sf::VertexArray& arr)
{
  mFloorLayer = arr;
}

void RenderComponent::addStaticActor(sf::Sprite& sprite)
{
  mStaticActor.push_back(sprite);
}

void RenderComponent::addDynamicActor(sf::Sprite& sprite)
{
  mDynamicActor.push_back(sprite);
}

void RenderComponent::render(sf::RenderWindow& window)
{
  window.draw(mFloorLayer, &(dr::Textures::get("floor_texture")));
  for (const auto& sa : mStaticActor) {
    window.draw(sa);
  }
  for (const auto& da : mDynamicActor) {
    window.draw(da);
  }
}