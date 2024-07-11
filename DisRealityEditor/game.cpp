#include "game.h"
#include "screen_manager.h"
#include "game_data.h"
#include <iostream>

void Game::createStartScreen() 
{
  ScreenManager::createScreen(GameData::Screens::MENU);
}

void Game::init() 
{
  try {
    dr::Textures::init(GameData::path::TextureListFile);
    dr::Fonts::init(GameData::path::FontListFile);
  }
  catch (const std::runtime_error& err) {
    std::cout << err.what() << std::endl;
  }
  sf::Font& mainFont = dr::Fonts::get("Main");
  //dr::Fonts::load("Main", "assets/font/brushtype.ttf");

  // sf::Text object for the buttons
  sf::Text buttonText;
  buttonText.setFont(mainFont);
  buttonText.setCharacterSize(50);
  buttonText.setFillColor(sf::Color::Black);

  buttonText.setString("About");
  dr::TextManager::load("AboutButton", buttonText);
  buttonText.setString("Exit");
  dr::TextManager::load("ExitButton", buttonText);
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