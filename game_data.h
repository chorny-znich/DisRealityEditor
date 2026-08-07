#pragma once
#include <SFML/System/Vector2.hpp>
#include <string_view>

namespace gd
{
  const sf::Vector2f GraphicsResolution{ 1920.f, 1080.f };

  namespace path
  {
    constexpr std::string_view ConfigPath = "config/editor_config.ini";
    constexpr std::string_view StringsPath = "data\\strings.ini";
  }
}