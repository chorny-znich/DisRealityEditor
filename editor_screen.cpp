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
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Realisation
	if (mExitEditor)
	{
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
			drawTilemapEditor();		
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
				dr::Textures::init(mProjectPath + "data/texture_list.ini");
				dr::SpriteDatabase::instance().init(mProjectPath + "data/tile_map.ini");
				mFloorAsset.init(dr::SpriteCategory::Floor);
				
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
}

/**
 * @brief 
 * @param window 
 */
void EditorScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
	if (mMapIsReady)
	{
		window.draw(mTilemapEditor->currentMap);
	}
	ImGui::SFML::Render(window);
}

