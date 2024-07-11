#include "screen_manager.h"
#include "menu_screen.h"
#include "about_screen.h"
#include <memory>

void ScreenManager::createScreen(GameData::Screens screenID)
{
	if (screenID == GameData::Screens::MENU) {
		std::unique_ptr<MenuScreen> pMenuScreen = std::make_unique<MenuScreen>();
		pMenuScreen->init();
		dr::GameState::addScreen(std::move(pMenuScreen));
	} 
	else if (screenID == GameData::Screens::ABOUT) {
		std::unique_ptr<AboutScreen> pAboutScreen = std::make_unique<AboutScreen>();
		pAboutScreen->init();
		dr::GameState::addScreen(std::move(pAboutScreen));
	}
}