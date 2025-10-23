#include "editor_screen.h"
#include <format>

int EditorScreen::floorTypeValue{ 0 };
int EditorScreen::levelObjectTypeValue{ 0 };
int EditorScreen::staticObjectTypeValue{ 0 };

/**
 * @brief Load the map from the ini file 
 * @param filename - name of the ini file with maps's data 
 */
void EditorScreen::loadMap(int mapIndex)
{
  mCurrentMap.loadFromFile(std::format("data/maps/map_{}.ini", mapIndex));
  mCurrentMap.loadEntries(std::format("data/maps/entry_{}.ini", mapIndex));
  mCurrentMap.createFloorMap();
  mCurrentMap.createLevelObjects();
  mCurrentMap.createStaticObjects();

  mRenderComponent.updateFloorLayer(mCurrentMap.getFloorMap());
  mRenderComponent.updateLevelLayer(mCurrentMap.getLevelObjects());
  mRenderComponent.updateStaticLayer(mCurrentMap.getStaticObjects());

  mInputComponent.setMapSize(mCurrentMap.getMapSize());

  mState = State::VIEW;
}

EditorScreen::EditorScreen()
{
}

void EditorScreen::init()
{
	
}

void EditorScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
  if (mState == State::VIEW) {
    if (key == sf::Keyboard::A) {
      mMainView.move({ 10, 0 });
    }
    else if (key == sf::Keyboard::D) {
      mMainView.move({ -10, 0 });
    }
    else if (key == sf::Keyboard::W) {
      mMainView.move({ 0, 10 });
    }
    else if (key == sf::Keyboard::S) {
      mMainView.move({ 0, -10 });
    }
  }
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
  /*ImGui::Begin("!");
  ImGui::Text(std::format("{}", mLevelTypeSetup).c_str());
  ImGui::End();*/

  if (mState == State::MENU) {
    ImGui::Begin("Menu");
    if (ImGui::Button("New", {200, 75})) {
      mState = State::CREATE_MAP;
      }
    if (ImGui::Button("Load",{ 200, 75 })) {
      mState = State::LOAD_MAP;
    }
    if (ImGui::Button("Exit", { 200, 75 })) {
      dr::GameState::destroyScreen();
    }
    ImGui::End();
  }

  else if (mState == State::CREATE_MAP) {
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

  else if (mState == State::LOAD_MAP) {
    static int mapIndex{ 0 };
    ImGui::Begin("Filename");
    ImGui::InputInt("Enter map's number", &mapIndex);
    if (ImGui::Button("Load")) {
      loadMap(mapIndex);
    }
    ImGui::End();
  }

  else if (mState == State::VIEW) {
    mInputComponent.update(dt);
    
    dr::Location& loc = mCurrentMap.getLocation(mInputComponent.getTilePosition().y *
      mCurrentMap.getMapSize().x + mInputComponent.getTilePosition().x);
   
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
    ImGui::Text(std::format("Passability: {}", loc.isPassable()).c_str());
    ImGui::Text(std::format("Entry: {}", loc.isEntry()).c_str());
    ImGui::End();
  }

  else if (mState == State::EDIT) {
    // Get the information of the edited location and form the data for the menus
    mInputComponent.update(dt);
    dr::Location& loc = mCurrentMap.getLocation(mInputComponent.getTilePosition().y *
      mCurrentMap.getMapSize().x + mInputComponent.getTilePosition().x);
    const std::string currentFloorLayer = loc.getFloorLayerId();
    const std::string currentLevelLayer = loc.getLevelLayerId();
    const std::string currentObjectLayer = loc.getObjectLayerId();

    static const char* floorType[]{ "dirt", "dirt_tile", "stone_tile", "stone_missing_tiles_e",
    "stone_uneven_e", "stone_uneven_n", "stone_uneven_w", "stone_uneven_s",
    "stone_tile_e", "stone_tile_n", "stone_tile_s", "stone_tile_w" };
    
    // Set the floor layer id to it's current value
    if (!mFloorTypeSetup) {
      for (size_t i = 0; i < 12; i++) {
        if (currentFloorLayer == floorType[i]) {
          floorTypeValue = static_cast<int>(i);
          break;
        }
      }
      mFloorTypeSetup = true;
    }
    
    static const char* levelObjectType[]{ "none", "stone_wall_corner_nw", "stone_wall_corner_ne", "stone_wall_corner_sw",
      "stone_wall_corner_se", "stone_wall_hn", "stone_wall_hs", "stone_wall_vw", "stone_wall_ve",
    "stone_wall_broken_w", "stone_wall_broken_e", "stone_wall_broken_n", "stone_wall_broken_s",
    "stone_wall_window_bars_e", "stone_wall_window_bars_w", "stone_wall_window_bars_n", "stone_wall_window_bars_s",
    "stone_wall_door_open_e", "stone_wall_door_open_w", "stone_wall_door_open_n", "stone_wall_door_open_s" };
    // Set the level layer id to it's current value
    if (!mLevelTypeSetup) {
      for (size_t i = 0; i < 21; i++) {
        if (currentLevelLayer == levelObjectType[i]) {
          levelObjectTypeValue = static_cast<int>(i);
          break;
        }
      }
      mLevelTypeSetup = true;
    }

    static const char* staticObjectType[]{ "none", "ladder_down", "barrels_stacked_h", "barrels_stacked_v",
      "stone_column_n", "stone_column_wood_n", "table_chairs_broken_n", "table_short_chairs_w"};
    // Set the static object layer id to it's current value
    if (!mStaticObjectSetup) {
      for (size_t i = 0; i < 8; i++) {
        if (currentObjectLayer == staticObjectType[i]) {
          staticObjectTypeValue = static_cast<int>(i);
          break;
        }
      }
      mStaticObjectSetup = true;
    }


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
    if (ImGui::Checkbox("Entry: ", &isEntry)) {
      loc.setEntry(true);
      loc.setPassability(1);
      dr::MapEntry newEntry;
      newEntry.setId(std::format("entry_{}_{}\n", mCurrentMap.getNumberOfEntries(), mCurrentMap.getMapIndex()));
      newEntry.setMapId(mCurrentMap.getMapIndex());
      newEntry.setPosition(loc.getPosition());
      mCurrentMap.createEntry(loc.getId(), std::move(newEntry));
    }
    if (ImGui::Button("Done")) {
      mCurrentMap.updateFloorMap(loc.getId(), floorType[floorTypeValue]);
      mRenderComponent.updateFloorLayer(mCurrentMap.getFloorMap());
      loc.setFloorLayerId(floorType[floorTypeValue]);
      loc.setLevelLayerId(levelObjectType[levelObjectTypeValue]);
      loc.setObjectLayerId(staticObjectType[staticObjectTypeValue]);

      // Add or delete the level object to/from the current map
      if (levelObjectType[levelObjectTypeValue] != "none") {
        mCurrentMap.addLevelObject(std::move(mCurrentMap.createLevelObject(loc.getId())));
        mRenderComponent.updateLevelLayer(mCurrentMap.getLevelObjects());
      }
      else {
        if (currentLevelLayer != "none") {
          mCurrentMap.deleteLevelObject(loc.getId());
          mRenderComponent.updateLevelLayer(mCurrentMap.getLevelObjects());
        }
      }
      // Add or delete the static object to/from the current map
      if (staticObjectType[staticObjectTypeValue] != "none") {
        mCurrentMap.addStaticObject(std::move(mCurrentMap.createStaticObject(loc.getId())));
        mRenderComponent.updateStaticLayer(mCurrentMap.getStaticObjects());
      }
      else {
        loc.setPassability(true);
        if (currentObjectLayer != "none") {
          mCurrentMap.deleteStaticObject(loc.getId());
          mRenderComponent.updateStaticLayer(mCurrentMap.getStaticObjects());
        }
      }
      if (!isEntry) {
        loc.setEntry(false);
      }
      isEntry = false;

      // Set passability of the edited location
      if (levelObjectType[levelObjectTypeValue] != "none" || staticObjectType[staticObjectTypeValue] != "none") {
        loc.setPassability(false);
      }
      if (loc.isEntry()) {
        loc.setPassability(true);
      }

      mInputComponent.finishEditMode();
      mFloorTypeSetup = false;
      mLevelTypeSetup = false;
      mStaticObjectSetup = false;
      mState = State::VIEW;
    }
    ImGui::End();
    // Setup map's entry
    if (loc.isEntry()) {
      static int linkedEntryMapIndex = 0;
      static int linkedEntryIndex = 0;
      dr::MapEntry& entry = mCurrentMap.getEntry(loc.getId());
      ImGui::Begin("Entry");
      ImGui::Text(std::format("ID: {}", entry.getId()).c_str());
      ImGui::Text(std::format("Map index: {}", entry.getMapId()).c_str());
      ImGui::Text(std::format("x: {}", entry.getPosition().x).c_str());
      ImGui::Text(std::format("y: {}", entry.getPosition().y).c_str());
      ImGui::InputInt("Linked entry map index", &(linkedEntryMapIndex));
      ImGui::InputInt("Linked entry index", &(linkedEntryIndex));
      if (ImGui::Button("Done")) {
        entry.setLinkedEntryId(std::format("entry_{}_{}", linkedEntryIndex, linkedEntryMapIndex).c_str());
      }
      ImGui::End();
    }
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