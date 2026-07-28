#include "asset_palette.h"
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <string>

void AssetPalette::init(dr::SpriteCategory type)
{
  mSpriteType = type;
  mSpriteIds = dr::SpriteDatabase::instance().getIdByCategory(type);
  if (!mSpriteIds.empty())
  {
    mSelectedId = mSpriteIds[0];
  }
  sf::RenderTexture canvas(BUTTON_SPRITE_SIZE);
  mButtonSprites.clear();

  for (const auto& id : mSpriteIds)
  {
    sf::Sprite sprite = dr::SpriteDatabase::instance().getSprite(id);
    float scaleX = BUTTON_SPRITE_SIZE.x / sprite.getLocalBounds().size.x;
    float scaleY = BUTTON_SPRITE_SIZE.y / sprite.getLocalBounds().size.y;
    sprite.setScale({ scaleX, scaleY });
    sprite.setPosition({0.f, 0.f});
    canvas.clear(sf::Color::Transparent);
    canvas.draw(sprite);
    canvas.display();
    sf::Texture texture = canvas.getTexture();
    mButtonSprites.insert({ id,texture });
  }
}

/**
 * @brief 
 * @param target 
 */
bool AssetPalette::draw()
{
  bool isAnyItemSelected = false;
  for (const auto& [id, texture] : mButtonSprites)
  {
    std::string buttonID = "##tile_" + std::to_string(id);
    sf::Vector2f buttonSize(static_cast<float>(BUTTON_SPRITE_SIZE.x), static_cast<float>(BUTTON_SPRITE_SIZE.y));
    if (ImGui::ImageButton(buttonID.c_str(), texture, buttonSize))
    {
      mSelectedId = id;
      isAnyItemSelected = true;
     }
  }
  return isAnyItemSelected;
}

/**
 * @brief 
 * @param id 
 * @return 
 */
const sf::Texture& AssetPalette::getSelectedTexture() const
{
  return mButtonSprites.at(mSelectedId);
}

const sf::Vector2f AssetPalette::getButtonSpriteSize() const
{
  return { static_cast<float>(BUTTON_SPRITE_SIZE.x), static_cast<float>(BUTTON_SPRITE_SIZE.y) };
}

uint16_t AssetPalette::getSelectedId() const
{
  return mSelectedId;
}

void AssetPalette::setSelectedId(uint16_t id)
{
  mSelectedId = id;
}

dr::SpriteCategory AssetPalette::getSpriteCategory() const
{
  return mSpriteType;
}