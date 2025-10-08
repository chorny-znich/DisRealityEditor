#include "editor_screen.h"

void EditorScreen::init()
{
	
}

void EditorScreen::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
}

void EditorScreen::inputHandler(sf::Mouse::Button button, bool isPressed, sf::Vector2i position, sf::RenderWindow* window)
{
	
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
      //mInputComponent.setMapSize(mCurrentMap.getMapSize());
      mState = State::VIEW;
    }
    ImGui::End();
  }
}

void EditorScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
	
}