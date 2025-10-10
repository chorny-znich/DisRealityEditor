#include "editor_screen.h"
#include <format>

EditorScreen::EditorScreen() :
  mRenderComponent(mCurrentMap.getLevelObjects())
{
}

void EditorScreen::init()
{
	
}

void EditorScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
}

void EditorScreen::inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window)
{
  if (mState == State::VIEW) {
    mInputComponent.inputHandler(position, window);
  }
  if (button == sf::Mouse::Right && mInputComponent.getCursorMode() == dr::CursorMode::MOVE) {
    mInputComponent.setCursorMode(dr::CursorMode::EDIT);
    mState = State::EDIT;
    mInputComponent.changeCursorColor(sf::Color::Red);
  }
}

void EditorScreen::update(sf::Time dt)
{
  if (mState == State::CREATE_MAP) {
    static const char* floorType[]{ "dirt", "dirt_tile", "stone_tile" };
    static int floorTypeValue{ 0 };
    static int mapIndex;
    static sf::Vector2i mapSize;
    ImGui::Begin("Create map");
    ImGui::InputInt("Map index", &(mapIndex));
    ImGui::InputInt("Map size x", &(mapSize.x));
    ImGui::InputInt("Map size y", &(mapSize.y));
    bool floorTypeCheck = ImGui::Combo("Floor type", &floorTypeValue, floorType, IM_ARRAYSIZE(floorType));
    if (ImGui::Button("Create new map")) {
      mCurrentMap.createMap(mapIndex, mapSize, floorType[floorTypeValue]);
      mRenderComponent.updateFloorLayer(mCurrentMap.getFloorMap());
      mInputComponent.setMapSize(mCurrentMap.getMapSize());
      mState = State::VIEW;
    }
    ImGui::End();
  }
  else if (mState == State::VIEW) {
    mInputComponent.update(dt);
    
    dr::Location& loc = mCurrentMap.getLocation(mInputComponent.getTilePosition().y *
      mCurrentMap.getMapSize().x + mInputComponent.getTilePosition().x);
    /*static const char* floorType[]{"dirt", "dirt_tile", "stone_tile"};
    static int floorTypeValue{ 0 };
    static const char* levelType[]{ "none", "stone_wall_corner_nw", "stone_wall_corner_ne", "stone_wall_corner_sw",
      "stone_wall_corner_se", "stone_wall_hn", "stone_wall_hs", "stone_wall_vw", "stone_wall_ve" };
    static int levelTypeValue{ 0 };
    */ 
    ImGui::Begin("Editor menu");
    if (ImGui::Button("Save map")) {
      mCurrentMap.saveMap("map_" + std::to_string(mCurrentMap.getMapIndex()));
      mCurrentMap.saveEntries("entry_" + std::to_string(mCurrentMap.getMapIndex()));
    }
    ImGui::End();
    
    ImGui::Begin("Location info");
    ImGui::Text(std::format("id: {}", loc.getId()).c_str());
    ImGui::Text(std::format("x: {}", loc.getPosition().x).c_str());
    ImGui::Text(std::format("y: {}", loc.getPosition().y).c_str());
    ImGui::Text(std::format("floor type: {}", loc.getFloorLayerId()).c_str());
    ImGui::Text(std::format("Level object type: {}", loc.getLevelLayerId()).c_str());
    ImGui::Text(std::format("Static object type: {}", loc.getObjectLayerId()).c_str());
    ImGui::Text(std::format("Entry: {}", loc.isEntry()).c_str());
    ImGui::End();
  }
  else if (mState == State::EDIT) {
    // Get the information of the edited location and form the data for the menus
    mInputComponent.update(dt);
    dr::Location& loc = mCurrentMap.getLocation(mInputComponent.getTilePosition().y *
      mCurrentMap.getMapSize().x + mInputComponent.getTilePosition().x);
    const std::string currentLevelLayer = loc.getLevelLayerId();
    const std::string currentObjectLayer = loc.getObjectLayerId();
    static const char* floorType[]{ "dirt", "dirt_tile", "stone_tile", "stone_missing_tiles_e",
    "stone_uneven_e", "stone_uneven_n", "stone_uneven_w", "stone_uneven_s" };
    static int floorTypeValue{ 0 };
    static const char* levelObjectType[]{ "none", "stone_wall_corner_nw", "stone_wall_corner_ne", "stone_wall_corner_sw",
      "stone_wall_corner_se", "stone_wall_hn", "stone_wall_hs", "stone_wall_vw", "stone_wall_ve",
    "stone_wall_broken_w", "stone_wall_broken_e", "stone_wall_broken_n", "stone_wall_broken_s",
    "stone_wall_window_bars_e", "stone_wall_window_bars_w", "stone_wall_window_bars_n", "stone_wall_window_bars_s" };
    static int levelObjectTypeValue{ 0 };
    static const char* staticObjectType[]{ "none", "ladder_down", "barrels_stacked_h", "barrels_stacked_v" };
    static int staticObjectTypeValue{ 0 };
    static bool isEntry = loc.isEntry();
    
    // Menus in the edit mode
    ImGui::Begin("Location info (Edit)");
    ImGui::Text(std::format("id: {}", loc.getId()).c_str());
    ImGui::Text(std::format("x: {}", loc.getPosition().x).c_str());
    ImGui::Text(std::format("y: {}", loc.getPosition().y).c_str());
    ImGui::Text(std::format("floor type: {}", loc.getFloorLayerId()).c_str());
    bool floorTypeCheck = ImGui::Combo("Floor type", &floorTypeValue, floorType, IM_ARRAYSIZE(floorType));
    ImGui::Text(std::format("Object type: {}", loc.getLevelLayerId()).c_str());
    bool levelObjectTypeCheck = ImGui::Combo("Level object type", &levelObjectTypeValue, levelObjectType, IM_ARRAYSIZE(levelObjectType));
    ImGui::Text(std::format("Static object type: {}", loc.getObjectLayerId()).c_str());
    bool staticObjectTypeCheck = ImGui::Combo("Static object type", &staticObjectTypeValue, staticObjectType, IM_ARRAYSIZE(staticObjectType));
    /*if (ImGui::Checkbox("Entry: ", &isEntry)) {
      loc.setEntry(true);
      dr::MapEntry newEntry;
      newEntry.setId(std::format("[entry_{}_{}]\n", mCurrentMap.getNumberOfEntries(), mCurrentMap.getMapIndex()));
      newEntry.setMapId(mCurrentMap.getMapIndex());
      newEntry.setPosition(loc.getPosition());
      mCurrentMap.createEntry(loc.getId(), std::move(newEntry));
    }*/
    if (ImGui::Button("Done")) {
      mCurrentMap.updateFloorMap(loc.getId(), floorType[floorTypeValue]);
      mRenderComponent.updateFloorLayer(mCurrentMap.getFloorMap());
      loc.setFloorLayerId(floorType[floorTypeValue]);
      loc.setLevelLayerId(levelObjectType[levelObjectTypeValue]);
      loc.setObjectLayerId(staticObjectType[staticObjectTypeValue]);

      // Add or delete the level object to/from the render component
      if (levelObjectType[levelObjectTypeValue] != "none") {
        mCurrentMap.addLevelObject(std::move(mCurrentMap.createLevelObject(loc.getId())));
      }
      else {
        if (currentLevelLayer != "none") {
          mCurrentMap.deleteLevelObject(loc.getId());
        }
      }
      // Add or delete the static object to/from the render component
     /* if (staticObjectType[staticObjectTypeValue] != "none") {
        mRenderComponent.addStaticObject(std::move(mCurrentMap.createStaticObject(loc.getId())));
        loc.setPassability(true);
      }
      else {
        loc.setPassability(false);
        if (currentObjectLayer != "none") {
          mRenderComponent.deleteStaticObject(loc.getId());
        }
      }*/
      if (!isEntry) {
        loc.setEntry(false);
      }
      isEntry = false;
      mInputComponent.finishEditMode();
      mState = State::VIEW;
    }
    ImGui::End();
  }
}

void EditorScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
	
  switch (mState) {
  case State::EDIT:
  case State::VIEW:
    mRenderComponent.render(window);
    mInputComponent.render(window);
    break;
  }
}