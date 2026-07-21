#pragma once
#include <disreality_engine.h>
#include <vector>

/**
 * @brief 
 */
class AssetPalette
{
public:
  void init(dr::SpriteCategory type);
  void render(sf::RenderTarget& target, sf::Vector2f pos);
private:
  std::vector<std::uint16_t> mSpriteIds;
  std::uint16_t mSelectedId{ 0 };
};