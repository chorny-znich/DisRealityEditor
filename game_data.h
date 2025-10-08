#pragma once
#include <string>
#include <SFML/System.hpp>

namespace GameData
{
  /**
 * @brief All pathes to the game files
*/
  inline namespace path {
    const std::string TextureListFile{ R"(data/texture_list.ini)" };
    const std::string FontListFile{R"(data/font_list.ini)"};
  }
  const sf::Vector2f GraphicResolution{ 1920.f, 1080.f };
  const sf::Vector2f TileSize{ 128.f, 128.f };
}