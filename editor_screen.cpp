#include "editor_screen.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <map>
#include <string>

/**
 * @brief Common structure for all "flat" ini editors
 */
struct EditorScreen::FlatIniEditor
{
	std::map<std::string, std::string> data;
	std::string selectedKey{};
	bool isEditing{ false };
	char bufferKey[128] = "";
	char bufferValue[1024] = "";
};

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
	mStringEditor(std::make_unique<FlatIniEditor>())
{
}

EditorScreen::~EditorScreen() = default;

/**
 * @brief Initialize screen resources
 */
void EditorScreen::init()
{
	ImGui::SFML::Init(dr::ImguiHelper::getWindow());
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
				mStringEditor->data.clear();

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
		drawFlatIniEditor("String Editor", *mStringEditor);
	}
}

/**
 * @brief 
 * @param window 
 */
void EditorScreen::render(sf::RenderWindow& window)
{
	window.setView(mMainView);
	ImGui::SFML::Render(window);
}

void EditorScreen::drawFlatIniEditor(const std::string& title, FlatIniEditor& editor)
{
	// Editor title
	ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), title.c_str());
	ImGui::Spacing();
	// Read only table
	if (ImGui::BeginTable("flatEditorTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
		ImGuiTableFlags_ScrollY, ImVec2(0.f, 300.f)))
	{
		ImGui::TableSetupColumn("ID");
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();
		ImGui::EndTable();
	}
}