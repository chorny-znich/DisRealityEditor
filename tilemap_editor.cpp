#include "editor_struct.h"
#include <imgui.h>
#include <imgui-SFML.h>

/**
 * @brief Visual editor for build the game maps
 * @param editor referense to the structure that has editor's data and settings
 */
void EditorScreen::drawTilemapEditor(float dt)
{
	if (ImGui::Begin("Tilemap editor", &mShowTilemapEditor))
	{
		if (tilemapUIState == TilemapUIStates::SELECT)
		{
			ImGui::Text("The Tilemap editor is empty. Choose: Create a new map or load a map from the file");
			if (ImGui::Button("Create new map", ImVec2(200, 50)))
			{
				ImGui::Text("For creating a new map");
				tilemapUIState = TilemapUIStates::CREATE;
			}

			if (ImGui::Button("Load map", ImVec2(200, 50)))
			{
				ImGui::Text("For loading a new map");
				tilemapUIState = TilemapUIStates::LOAD;
			}
		}

		// Create a new map with ID, a map's size in tiles and choose a floor tile to fill the map
		else if (tilemapUIState == TilemapUIStates::CREATE)
		{
			ImGui::Text("Create a new map");

			static int mapIndex{ 0 };
			static sf::Vector2i mapSize{ 0, 0 };
			static int selectedFloorItem{ 0 };
			static bool showPopupPalette{ false };

			ImGui::InputInt("Map ID", &mapIndex);
			ImGui::InputInt("Map width", &mapSize.x);
			ImGui::InputInt("Map height", &mapSize.y);
			
			if (mapSize.x < 1) mapSize.x = 1;
			if (mapSize.y < 1) mapSize.y = 1;
			
			ImGui::Text("Choose floor tile");
			ImGui::SameLine();
			if (ImGui::ImageButton("##floor preview", mFloorAsset.getSelectedTexture(), mFloorAsset.getButtonSpriteSize()))
			{
				showPopupPalette = true;
			}

			if (showPopupPalette)
			{
				ImGui::Begin("Choose a floor tile", &showPopupPalette, ImGuiWindowFlags_AlwaysAutoResize);
				if (mFloorAsset.draw())
				{
					showPopupPalette = false;
				}
				ImGui::End();
			}

			ImGui::Separator();
			if (ImGui::Button("Create map", ImVec2(200, 50)))
			{
				uint16_t selectedID = mFloorAsset.getSelectedId();
				dr::SpriteInfo info = dr::SpriteDatabase::instance().getSpriteInfo(selectedID);
				mTilemapEditor->currentMap.setFloorTextureId(info.textureId);
				mTilemapEditor->currentMap.createMap(mapIndex, 
					{static_cast<unsigned int>(mapSize.x), static_cast<unsigned int>(mapSize.y)}, selectedID);
				
				mMapIsReady = true;
				mCursor.setMapSize({ mapSize.x, mapSize.y });
				tilemapUIState = TilemapUIStates::EDIT;
			}
			ImGui::SameLine();
			if (ImGui::Button("Back", ImVec2(200, 50)))
			{
				tilemapUIState = TilemapUIStates::SELECT;
			}
		}

		else if (tilemapUIState == TilemapUIStates::LOAD)
		{
			ImGui::Text("Load a map");
			if (ImGui::Button("BACK", ImVec2(200, 50)))
			{
				tilemapUIState = TilemapUIStates::SELECT;
			}
		}

		else if (tilemapUIState == TilemapUIStates::EDIT)
		{
			ImGui::Text("Edit a map");
			mCursor.update(dt);
		}

		ImGui::End();
	}
}