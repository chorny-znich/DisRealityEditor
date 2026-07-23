#pragma once
#include "editor_screen.h"
#include <disreality_engine.h>
#include <string>
#include <map>

/**
 * @brief Common structure for all "flat" ini editors
 */
struct EditorScreen::FlatIniEditor
{
	std::map<std::string, std::string> data;
	std::string selectedKey{};
	bool isAdding{ false };
	bool isEditing{ false };
	char bufferKey[128] = "";
	char bufferValue[1024] = "";
	std::string relativePath{};
	std::string sectionName{};
};

/**
 * @brief Visual map editor
 */
struct EditorScreen::TilemapEditor
{
	dr::Map currentMap;
	int selectedFloorIdx;
	int selectedLevelObjIdx;
	int selectedstaticObjIdx;
};