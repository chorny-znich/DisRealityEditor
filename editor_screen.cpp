#include "editor_screen.h"
#include "editor_struct.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <map>
#include <string>

/**
 * @brief For handling Events in handleInput method
 */
struct EditorScreen::ScreenInputVisitor
{
	EditorScreen& screen;
	sf::RenderWindow& window;

	void clearUI()
	{
		window.setMouseCursor(dr::CursorManager::get("arrow"));
	}

	void operator()(const sf::Event::MouseMoved& mouseMoved)
	{
		sf::Vector2f mouseViewCoords = window.mapPixelToCoords(mouseMoved.position);
		screen.mCursor.handleInput(mouseMoved.position, window);
	}

	/**
	 * @brief Mouse button clicked event
	 * @param
	 */
	void operator()(const sf::Event::MouseButtonPressed mouseButton)
	{
		if (mouseButton.button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mouseViewCoords = window.mapPixelToCoords(mouseButton.position);
			if (!ImGui::GetIO().WantCaptureMouse)
			{
				screen.mTilemapEditor->selectedTile = screen.mCursor.getTilePosition();
				screen.mCursor.startEdit();
			}
		}
		if (mouseButton.button == sf::Mouse::Button::Right)
		{
			screen.mTilemapEditor->selectedTile = screen.mCursor.getTilePosition();
			screen.mCursor.startEdit();
			screen.mTileEdited = true;
		}
		if (mouseButton.button == sf::Mouse::Button::Middle)
		{
			screen.mCursor.finishEdit();
			screen.mTileEdited = false;
		}
	}

	void operator()(const auto&) {};
};

EditorScreen::EditorScreen() :
	mStringEditor(std::make_unique<FlatIniEditor>()),
	mTilemapEditor(std::make_unique<TilemapEditor>())
{
}

EditorScreen::~EditorScreen() = default;

/**
 * @brief Initialize screen resources
 */
void EditorScreen::init()
{
	ImGui::SFML::Init(dr::ImguiHelper::getWindow());

	ImGuiIO& io = ImGui::GetIO();
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/font/arial.ttf", 16.f, nullptr,
		io.Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();

	float topOffset = 40.f / gd::GraphicsResolution.y;
	sf::FloatRect viewBounds({ 0.f, 0.f }, { gd::GraphicsResolution.x, gd::GraphicsResolution.y });
	mTilemapView = sf::View(viewBounds);
	mTilemapView.setViewport(sf::FloatRect({ 0.f, topOffset }, { 1.f, 1.f - topOffset }));

	// read from the configuration file
	dr::IniDocument doc = dr::loadIniDocument(gd::path::ConfigPath.data());
	dr::Section section = doc.getSection("project");
	mProjectPath = section.at("last_project");
	std::filesystem::current_path(mProjectPath);
	changeProjectPath();
}

void EditorScreen::handleInput(const sf::Event& event, sf::RenderWindow& window)
{
	ImGui::SFML::ProcessEvent(window, event);
	event.visit(ScreenInputVisitor{ *this, window });
}

/**
 * @brief draw ImGui interface
 * @param dt 
 */
void EditorScreen::update(float dt)
{
	ImGui::SFML::Update(dr::ImguiHelper::getWindow(), dr::ImguiHelper::getTime());

	// Draw ImGui menus
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open project"))
			{
				mShowOpenProjectModalWindow = true;
				strcpy_s(mProjectPathBuffer, sizeof(mProjectPathBuffer), mProjectPath.c_str());
			}
			if (ImGui::MenuItem("Exit", "Esc"))
			{
				mExitEditor = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editors"))
		{
			if (ImGui::MenuItem("Strings"))
			{
				mShowStringEditor = true;
				if (mProjectPath != "")
				{
					mStringEditor->relativePath = gd::path::StringsPath.data();
					mStringEditor->sectionName = "strings";
					std::string stringPath = mProjectPath + mStringEditor->relativePath;
					mStringEditor->data.clear();
					try
					{
						dr::IniDocument doc = dr::loadIniDocument(stringPath);
						for (const auto& str : doc.getSection(mStringEditor->sectionName))
						{
							mStringEditor->data.insert(str);
						}
					}
					catch (std::exception& e)
					{
						std::cout << std::format("Editor error: {}, details: {}\n", mProjectPath, e.what());
					}
				}
			}
			if (ImGui::MenuItem("TilemapEditor"))
			{
				mShowTilemapEditor = true;
				mCursor.init();
			}
			ImGui::EndMenu();
		}

		// Show a path to the current project
		float offsetX = ImGui::GetWindowWidth() - ImGui::CalcTextSize(mProjectPath.c_str()).x - 25.f;
		ImGui::SameLine(offsetX, 0.f);
		ImGui::TextDisabled(mProjectPath.c_str());

		ImGui::EndMainMenuBar();
	}

	// Realisation
	if (mExitEditor)
	{
		std::filesystem::current_path(EDITOR_PATH);
		dr::Textures::init(mProjectPath + "data/texture_list.ini");
		dr::SpriteDatabase::instance().init(mProjectPath + "data/tile_map.ini");
		dr::ScreenManager::destroyScreen();
	}

	if (mShowStringEditor)
	{
		if (ImGui::Begin("String editor (strings.ini)", &mShowStringEditor))
		{
			drawFlatIniEditor("String Editor", *mStringEditor);
		}
		ImGui::End();
	}

	if (mShowTilemapEditor)
	{
			drawTilemapEditor(dt);		
	}

	// Choose the path to the current project
	if (mShowOpenProjectModalWindow)
	{
		ImGui::OpenPopup("Select project path");
		mShowOpenProjectModalWindow = false;
	}

	if (ImGui::BeginPopupModal("Select project path", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter the path to your game project folder");
		ImGui::InputText("Path to project", mProjectPathBuffer, sizeof(mProjectPathBuffer));
		bool isEmpty = mProjectPathBuffer[0] == '\0';
		ImGui::BeginDisabled(isEmpty);
			if (ImGui::Button("Select"))
			{
				mProjectPath = mProjectPathBuffer;
				if (mProjectPath != "" && mProjectPath.back() != '\\')
				{
					mProjectPath += '\\';
				}
				std::filesystem::current_path(mProjectPath);
				changeProjectPath();

				// save the path to the current project to the config file
				const std::string CONFIG_PATH = gd::path::ConfigPath.data();
				std::string pathToEditor = EDITOR_PATH + '/' + CONFIG_PATH;
				dr::IniDocument doc = dr::loadIniDocument(pathToEditor);
				doc.addKeyValuePair("project", "last_project", mProjectPath);
				dr::saveIniDocument(pathToEditor, doc);
				
				ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	/*ImGui::Begin("Mouse coords");
	ImGui::Text(std::format("x:{}\ny:{}", sf::Mouse::getPosition().x, sf::Mouse::getPosition().y).c_str());
	ImGui::End();*/
	/*
	ImGui::Begin("Tile cursor coords");
	ImGui::Text(std::format("tile x:{}\ntile y:{}", mCursor.getTilePosition().x, 
		mCursor.getTilePosition().y).c_str());
	ImGui::End();
	*/
}

/**
 * @brief 
 * @param window 
 */
void EditorScreen::render(sf::RenderWindow& window)
{
	window.setView(mTilemapView);
	if (mMapIsReady)
	{
		window.draw(mTilemapEditor->currentMap);
		mCursor.render(window);
	}
	window.setView(mMainView);
	ImGui::SFML::Render(window);
}