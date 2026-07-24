#pragma once
#include <disreality_engine.h>
#include <vector>
#include <unordered_map>

/**
 * @brief 
 */
class AssetPalette
{
public:
  void init(dr::SpriteCategory type);
  bool draw();

  const sf::Texture& getSelectedTexture() const;
  const sf::Vector2f getButtonSpriteSize() const;
  uint16_t getSelectedId() const;
private:
  const sf::Vector2u BUTTON_SPRITE_SIZE{ 64u, 64u };
  std::vector<std::uint16_t> mSpriteIds;
  std::uint16_t mSelectedId{ 0 };
  std::unordered_map<uint16_t, sf::Texture> mButtonSprites;
  
};