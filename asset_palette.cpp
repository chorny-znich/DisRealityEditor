#include "asset_palette.h"

void AssetPalette::init(dr::SpriteCategory type)
{
  mSpriteIds = dr::SpriteDatabase::instance().getIdByCategory(type);
}

/**
 * @brief 
 * @param target 
 */
void AssetPalette::render(sf::RenderTarget& target, sf::Vector2f pos)
{
  sf::Vector2f currentPos{ pos };
  for (const auto& id : mSpriteIds)
  {
    sf::Sprite sprite = dr::SpriteDatabase::instance().getSprite(id);
    sprite.setPosition(currentPos);
    target.draw(sprite);
    currentPos.x += 150.f;
  }
}
