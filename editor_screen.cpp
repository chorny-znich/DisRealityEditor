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
	bool isAdding{ false };
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
	mStringEditor->data["back_button"] = "BACK";
	mStringEditor->data["editor_button"] = "EDITOR";
	mStringEditor->data["about_button"] = "ABOUT";
	mStringEditor->data["exit_button"] = "бшунд";
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

/**
 * @brief Display an editor for key-value pairs (string-string)
 * @param title name of the editor displayed at the top
 * @param editor a smart pointer to the specific editor as Strings or a list of textures
 */
void EditorScreen::drawFlatIniEditor(const std::string& title, FlatIniEditor& editor)
{
	// Add a new record
	if (ImGui::Button("Add record"))
	{
		editor.isAdding = true;
	}
	ImGui::Separator();

	// Display table with records
	if (ImGui::BeginTable(title.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable |
		ImGuiTableFlags_RowBg))
	{
		// Display header of the table
		ImGui::TableSetupColumn("ID");
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();
		// Display rows
		for (const auto& [key, value] : editor.data)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			bool isSelected = (key == editor.selectedKey);
			if (ImGui::Selectable(key.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
			{
				editor.selectedKey = key;
				editor.isEditing = true;
				strcpy_s(editor.bufferKey, sizeof(editor.bufferKey), key.c_str());
				strcpy_s(editor.bufferValue, sizeof(editor.bufferValue), value.c_str());
			}
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(value.c_str());
		}
		ImGui::EndTable();
	}

	// Add a new record
	if (editor.isAdding)
	{
		editor.selectedKey = "";
		editor.bufferKey[0] = '\0';
		editor.bufferValue[0] = '\0';

		ImGui::Separator();
		ImGui::Text("Add new record");
		ImGui::InputText("Key ID", editor.bufferKey, sizeof(editor.bufferKey));
		ImGui::InputText("Value data", editor.bufferValue, sizeof(editor.bufferValue));

		if (ImGui::Button("Apply"))
		{
			editor.data[editor.bufferKey] = editor.bufferValue;
			editor.isAdding = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			editor.isAdding = false;
		}
	}

	// Edit selected record
  if (editor.isEditing)
  {
    ImGui::Separator();
    ImGui::Text("Edit selected record:");
    ImGui::InputText("Key ID", editor.bufferKey, sizeof(editor.bufferKey));
    ImGui::InputText("Value data", editor.bufferValue, sizeof(editor.bufferValue));

    // Save or cancel changes
    if (ImGui::Button("Apply"))
    {
      editor.data[editor.selectedKey] = editor.bufferValue;
      editor.isEditing = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      editor.isEditing = false;
    }
  }
}