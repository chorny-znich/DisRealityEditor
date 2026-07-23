#include "editor_struct.h"
#include <imgui.h>
#include <imgui-SFML.h>

/**
 * @brief Visual editor for build the game maps
 * @param editor referense to the structure that has editor's data and settings
 */
void EditorScreen::drawTilemapEditor()
{
	if (ImGui::Begin("Tilemap editor", &mShowTilemapEditor))
	{
		if (ImGui::BeginTabBar("Tilemap tabs"))
		{
			if (ImGui::BeginTabItem("New map"))
			{
				ImGui::Text("For creating a new map");
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Load map"))
			{
				ImGui::Text("For loading a new map");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	ImGui::End();
	}
}