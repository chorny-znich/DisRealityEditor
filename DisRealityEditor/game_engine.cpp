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
  sf::Font& mainFont = dr::Fonts::get("main");

  // sf::Text object for the buttons
  sf::Text buttonText;
  buttonText.setFont(mainFont);
  buttonText.setCharacterSize(50);
  buttonText.setFillColor(sf::Color::Black);

  buttonText.setString("Editor");
  dr::TextManager::load("editor_button", buttonText);
  buttonText.setString("About");
  dr::TextManager::load("about_button", buttonText);
  buttonText.setString("Exit");
  dr::TextManager::load("exit_button", buttonText);
  buttonText.setString("Back");
  dr::TextManager::load("back_button", buttonText);

  // sf::Text object for text on the about screen
  sf::Text aboutText;
  aboutText.setFont(mainFont);
  aboutText.setCharacterSize(75);
  aboutText.setFillColor(sf::Color::Black);

  aboutText.setString("This is a map editor for games build on the DisReality game framework");
  dr::TextManager::load("description", aboutText);

  aboutText.setCharacterSize(25);
  aboutText.setString("version 0.1.0");
  dr::TextManager::load("version", aboutText);
}