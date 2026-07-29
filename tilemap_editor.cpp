#include "editor_struct.h"
#include <imgui.h>
#include <imgui-SFML.h>

/**
 * @brief Visual editor for build the game maps
 * @param editor referense to the structure that has editor's data and settings
 */
void EditorScreen::drawTilemapEditor(float dt)
{
  static bool showPopupPalette{ false };

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



      ImGui::Separator();
      if (ImGui::Button("Create map", ImVec2(200, 50)))
      {
        uint16_t selectedID = mFloorAsset.getSelectedId();
        dr::SpriteInfo info = dr::SpriteDatabase::instance().getSpriteInfo(selectedID);
        mTilemapEditor->currentMap.setFloorTextureId(info.textureId);
        mTilemapEditor->currentMap.createMap(mapIndex,
          { static_cast<unsigned int>(mapSize.x), static_cast<unsigned int>(mapSize.y) }, selectedID);

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
      if (mTilemapEditor->selectedTile.x != -1)
      {
        ImGui::Begin("Tile inspector");
        ImGui::SeparatorText("");
        ImGui::Text(std::format("Tile position\nx:{}\ty:{}", mTilemapEditor->selectedTile.x,
          mTilemapEditor->selectedTile.y).c_str());

        auto mapWidth = mTilemapEditor->currentMap.getMapSize().x;
        auto selectedID = mTilemapEditor->selectedTile.y * mapWidth +
          mTilemapEditor->selectedTile.x;
        auto& tileID = mTilemapEditor->currentMap.getLocation(selectedID).mFloorLayerId;
        mFloorAsset.setSelectedId(tileID);
        ImGui::Text("Floor tile:");
        if (ImGui::ImageButton("##floor_tile_inspector", mFloorAsset.getSelectedTexture(),
          mFloorAsset.getButtonSpriteSize()))
        {
          ImGui::OpenPopup("FloorPalettePopup");
          //showPopupPalette = true;
        }
        ImGui::Text("Level tile:");
        if (ImGui::ImageButton("##level_tile_inspector", mLevelObjectAsset.getSelectedTexture(),
          mLevelObjectAsset.getButtonSpriteSize()))
        {
          ImGui::OpenPopup("level_object_palette_popup");
          //showPopupPalette = true;
        }

        if (ImGui::BeginPopup("FloorPalettePopup", ImGuiWindowFlags_AlwaysAutoResize))
        {
          if (mFloorAsset.draw())
          {
            showPopupPalette = false;

            if (tilemapUIState == TilemapUIStates::EDIT)
            {
              auto mapWidth = mTilemapEditor->currentMap.getMapSize().x;
              auto selectedID = mTilemapEditor->selectedTile.y * mapWidth +
                mTilemapEditor->selectedTile.x;
              mTilemapEditor->currentMap.getLocation(selectedID).mFloorLayerId = mFloorAsset.getSelectedId();
              mTilemapEditor->currentMap.updateFloorMap(selectedID, mFloorAsset.getSelectedId());
              ImGui::CloseCurrentPopup();
            }
          }
          ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("level_object_palette_popup", ImGuiWindowFlags_AlwaysAutoResize))
        {
          if (mLevelObjectAsset.draw())
          {
            if (tilemapUIState == TilemapUIStates::EDIT)
            {
              auto mapWidth = mTilemapEditor->currentMap.getMapSize().x;
              auto selectedID = mTilemapEditor->selectedTile.y * mapWidth +
                mTilemapEditor->selectedTile.x;
              dr::Location& loc = mTilemapEditor->currentMap.getLocation(selectedID);
              uint16_t currentLayerID = loc.mLevelLayerId;
              uint16_t newLayerId = mLevelObjectAsset.getSelectedId();
              loc.mLevelLayerId = newLayerId;
              if (currentLayerID != 0)
              {
                mTilemapEditor->currentMap.deleteLevelObject(loc.mId);
              }
              if (newLayerId != 0)
              {
                mTilemapEditor->currentMap.addLevelObject(
                  std::move(mTilemapEditor->currentMap.createLevelObject(loc.mLevelLayerId)));
              }
            }
            ImGui::CloseCurrentPopup();
          }
        }
        ImGui::EndPopup();
      }

      ImGui::End();
    }

    mCursor.update(dt);
  }

  ImGui::End();
}