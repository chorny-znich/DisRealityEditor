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
			if (ImGui::ImageButton("##floor preview", mFloorPalette.getSelectedTexture(), mFloorPalette.getButtonSpriteSize()))
			{
				showPopupPalette = true;
			}

			ImGui::Separator();
			if (ImGui::Button("Create map", ImVec2(200, 50)))
			{
				uint16_t selectedID = mFloorPalette.getSelectedId();
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

		// Load a map from the file by its ID
		else if (tilemapUIState == TilemapUIStates::LOAD)
		{
			static int mapIndex{ 0 };
			ImGui::Text("Load the map from the file");
			ImGui::InputInt("Map ID", &mapIndex);
			
			if (ImGui::Button("Load", ImVec2(200, 50)))
			{
				mMapManager.loadMap(mapIndex);
				mTilemapEditor->currentMap = std::move(mMapManager.getCurrentMap());
				mMapIsReady = true;
				mCursor.setMapSize({ static_cast<int>(mMapManager.getCurrentMap().getMapSize().x), 
					static_cast<int>(mMapManager.getCurrentMap().getMapSize().y) });
				tilemapUIState = TilemapUIStates::EDIT;
			}

			if (ImGui::Button("BACK", ImVec2(200, 50)))
			{
				tilemapUIState = TilemapUIStates::SELECT;
			}
		}

		else if (tilemapUIState == TilemapUIStates::EDIT)
		{
			if (ImGui::Button("Save map"))
			{
				mMapManager.saveMap(mTilemapEditor->currentMap);
			}

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
				mFloorPalette.setSelectedId(tileID);
				ImGui::Text("Floor tile:");
				if (ImGui::ImageButton("##floor_tile_inspector", mFloorPalette.getSelectedTexture(),
					mFloorPalette.getButtonSpriteSize()))
				{
					ImGui::OpenPopup("FloorPalettePopup");
				}
				ImGui::Text("Architecture tile:");
				if (ImGui::ImageButton("##architecture_tile_inspector", mArchitecturePalette.getSelectedTexture(),
					mArchitecturePalette.getButtonSpriteSize()))
				{
					ImGui::OpenPopup("architecture_palette_popup");
				}

				ImGui::Text("Decoration tile:");
				if (ImGui::ImageButton("##decoration_tile_inspector", mDecorationPalette.getSelectedTexture(),
					mDecorationPalette.getButtonSpriteSize()))
				{
					ImGui::OpenPopup("decoration_palette_popup");
				}

				// Flag for the location passability
				bool locIsPassable = mTilemapEditor->currentMap.getLocation(selectedID).mPassable;
				if (ImGui::Checkbox("Location is passable", &locIsPassable))
				{
					mTilemapEditor->currentMap.getLocation(selectedID).mPassable = locIsPassable ? 1 : 0;
				}

				// Flag for the transfer location
				bool locIsTransfer = mTilemapEditor->currentMap.getLocation(selectedID).isTransfer;
				if (ImGui::Checkbox("Location is transfer", &locIsTransfer))
				{
					mTilemapEditor->currentMap.getLocation(selectedID).isTransfer = locIsTransfer ? 1 : 0;
					if (locIsTransfer)
					{
						ImGui::OpenPopup("transfer_popup");
					}
					else
					{
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetMapId = 0;
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetTilePos = {0, 0};
					}
				}

				// Edit the transfer location data if location is  a transfer
				if (locIsTransfer)
				{
					if (ImGui::Button("Edit transfer location"))
					{
						ImGui::OpenPopup("transfer_popup");
					}
				}

				if (ImGui::BeginPopup("FloorPalettePopup", ImGuiWindowFlags_AlwaysAutoResize))
				{
					if (mFloorPalette.draw())
					{
						showPopupPalette = false;

						if (tilemapUIState == TilemapUIStates::EDIT)
						{
							auto mapWidth = mTilemapEditor->currentMap.getMapSize().x;
							auto selectedID = mTilemapEditor->selectedTile.y * mapWidth +
								mTilemapEditor->selectedTile.x;
							mTilemapEditor->currentMap.getLocation(selectedID).mFloorLayerId = mFloorPalette.getSelectedId();
							mTilemapEditor->currentMap.updateFloorMap(selectedID, mFloorPalette.getSelectedId());
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopup("architecture_palette_popup", ImGuiWindowFlags_AlwaysAutoResize))
				{
					if (mArchitecturePalette.draw())
					{
						if (tilemapUIState == TilemapUIStates::EDIT)
						{
							auto mapWidth = mTilemapEditor->currentMap.getMapSize().x;
							auto selectedID = mTilemapEditor->selectedTile.y * mapWidth +
								mTilemapEditor->selectedTile.x;
							dr::Location& loc = mTilemapEditor->currentMap.getLocation(selectedID);
							uint16_t currentLayerID = loc.mArchitectureLayerId;
							uint16_t newLayerId = mArchitecturePalette.getSelectedId();

							if (newLayerId != 0)
							{
								if (currentLayerID != 0)
								{
									loc.mArchitectureLayerId = 0;
									mTilemapEditor->currentMap.deleteArchitectureActor(loc.mId);
								}
								loc.mArchitectureLayerId = newLayerId;
								loc.mPassable = 0;
								mTilemapEditor->currentMap.addArchitectureActor(mTilemapEditor->currentMap.createArchitectureActor(loc.mId));
							}

							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}

				if (ImGui::BeginPopup("decoration_palette_popup", ImGuiWindowFlags_AlwaysAutoResize))
				{
					if (mDecorationPalette.draw())
					{
						if (tilemapUIState == TilemapUIStates::EDIT)
						{
							auto mapWidth = mTilemapEditor->currentMap.getMapSize().x;
							auto selectedID = mTilemapEditor->selectedTile.y * mapWidth +
								mTilemapEditor->selectedTile.x;
							dr::Location& loc = mTilemapEditor->currentMap.getLocation(selectedID);
							uint16_t currentLayerID = loc.mDecorationLayerId;
							uint16_t newLayerId = mDecorationPalette.getSelectedId();

							if (newLayerId != 0)
							{
								if (currentLayerID != 0)
								{
									loc.mDecorationLayerId = 0;
									mTilemapEditor->currentMap.deleteDecorationActor(loc.mId);
								}
								loc.mDecorationLayerId = newLayerId;
								loc.mPassable = 0;
								mTilemapEditor->currentMap.addDecorationActor(mTilemapEditor->currentMap.createDecorationActor(loc.mId));
							}

							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}

				// Popup for the map transfer settings
				if (ImGui::BeginPopupModal("transfer_popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					int transferMapIndex = mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetMapId;
					int targetX = mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetTilePos.x;
					int targetY = mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetTilePos.y;

					if (ImGui::InputInt("Map index transfer to", &transferMapIndex))
					{
						transferMapIndex = std::clamp(transferMapIndex, 1, 1000);
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetMapId = transferMapIndex;
					}
					
					if (ImGui::InputInt("x coord of tile map", &targetX))
					{
						targetX = std::clamp(targetX, 1, 1000);
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetTilePos.x = targetX;
					}
					
					if (ImGui::InputInt("y coord of tile map", &targetY))
					{
						targetY = std::clamp(targetY, 1, 1000);
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetTilePos.y = targetY;
					}
					
					if (ImGui::Button("Apply"))
					{
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetMapId = transferMapIndex;
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetTilePos.x = targetX;
						mTilemapEditor->currentMap.getLocation(selectedID).mapTransfer.targetTilePos.y = targetY;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						ImGui::CloseCurrentPopup();
						mTilemapEditor->currentMap.getLocation(selectedID).isTransfer = 0;
					}

					ImGui::EndPopup();
				}

				ImGui::End();
			}

			// Move the Editor view
			sf::Vector2f tilemapViewMovement{ 0.f, 0.f };
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) tilemapViewMovement.x -= TILEMAP_VIEW_SPEED * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) tilemapViewMovement.x += TILEMAP_VIEW_SPEED * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) tilemapViewMovement.y -= TILEMAP_VIEW_SPEED * dt;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) tilemapViewMovement.y += TILEMAP_VIEW_SPEED * dt;
			mTilemapView.move(tilemapViewMovement);

			mCursor.update(dt);
		}

		ImGui::End();
	}

	

	if (showPopupPalette)
	{
		ImGui::Begin("Choose a floor tile", &showPopupPalette, ImGuiWindowFlags_AlwaysAutoResize);
		if (mFloorPalette.draw())
		{
			showPopupPalette = false;

			if (tilemapUIState == TilemapUIStates::EDIT)
			{
				auto mapWidth = mTilemapEditor->currentMap.getMapSize().x;
				auto selectedID = mTilemapEditor->selectedTile.y * mapWidth +
					mTilemapEditor->selectedTile.x;
				mTilemapEditor->currentMap.getLocation(selectedID).mFloorLayerId = mFloorPalette.getSelectedId();
				mTilemapEditor->currentMap.updateFloorMap(selectedID, mFloorPalette.getSelectedId());
			}
		}
		ImGui::End();
	}
}

/**
 * @brief Initialize resources from the current project opened in the editor 
 */
void EditorScreen::changeProjectPath()
{
	dr::Textures::init(mProjectPath + "data/texture_list.ini");
	dr::SpriteDatabase::instance().init(mProjectPath + "data/tile_map.ini");
	mFloorPalette.init(dr::SpriteCategory::Floor);
	mArchitecturePalette.init(dr::SpriteCategory::Architecture);
	mDecorationPalette.init(dr::SpriteCategory::Decoration);
}