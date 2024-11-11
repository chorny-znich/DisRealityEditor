#pragma once

#include "game_data.h"

class ScreenManager {
public:
	ScreenManager() = delete;
	static void createScreen(GameData::Screens screenId);
};