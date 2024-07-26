#include "game_engine.h"
#include "screen_manager.h"
#include "game_data.h"
#include <iostream>

void GameEngine::createStartScreen() 
{
  ScreenManager::createScreen(GameData::Screens::MENU);
}

void GameEngine::init() 
{
  try {
    dr::Textures::init(GameData::path::TextureListFile);
    dr::Fonts::init(GameData::path::FontListFile);
  }
  catch (const std::runtime_error& err) {
    std::cout << err.what() << std::endl;
  }
  sf::Font& mainFont = dr::Fonts::get("Main");

  // sf::Text object for the buttons
  sf::Text buttonText;
  buttonText.setFont(mainFont);
  buttonText.setCharacterSize(50);
  buttonText.setFillColor(sf::Color::Black);

  buttonText.setString("Create map");
  dr::TextManager::load("create_map_button", buttonText);
  buttonText.setString("Show map");
  dr::TextManager::load("show_map_button", buttonText);
  buttonText.setString("Exit");
  dr::TextManager::load("exit_button", buttonText);
  buttonText.setString("Back");
  dr::TextManager::load("BackButton", buttonText);

  // sf::Text object for text on the about screen
  sf::Text aboutText;
  aboutText.setFont(mainFont);
  aboutText.setCharacterSize(75);
  aboutText.setFillColor(sf::Color::Black);

  aboutText.setString("This is a template for all apps build on the DisReality game framework");
  dr::TextManager::load("AboutText_1", aboutText);
}