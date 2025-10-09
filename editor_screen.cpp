#include "editor_screen.h"
#include <format>

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
    */ /*
    ImGui::Begin("Editor menu");
    if (ImGui::Button("Save map")) {
      mCurrentMap.saveMap("map_" + std::to_string(mCurrentMap.getMapIndex()));
      mCurrentMap.saveEntries("entry_" + std::to_string(mCurrentMap.getMapIndex()));
    }
    ImGui::End();
    */
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