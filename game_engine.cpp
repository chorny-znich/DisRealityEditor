#include "game_engine.h"
#include "main_menu_screen.h"
#include <disreality_engine.h>

void GameEngine::init()
{
  dr::Engine::init();
}

/**
 * @brief Create the start screen. Usually a screen with the main menu
 */
void GameEngine::createStartScreen()
{
  dr::ScreenManager::addScreen<MainMenuScreen>("main_menu_screen");
}