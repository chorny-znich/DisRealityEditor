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
	std::string relativePath{};
	std::string sectionName{};
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
		editor.isEditing = false;
		editor.selectedKey = "";
	}

	ImGui::SameLine();

	bool isNoSelected = editor.selectedKey == "";
	ImGui::BeginDisabled(isNoSelected);
	if (ImGui::Button("Edit record"))
	{
		editor.isEditing = true;
		editor.isAdding = false;
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete record"))
	{
		if (editor.selectedKey != "")
		{
			editor.data.erase(editor.selectedKey);
			resetEditorState(editor);
		}
	}
	ImGui::EndDisabled();
	ImGui::Separator();

	// Display table with records
	if (ImGui::BeginChild("TablePart", ImVec2(0.f, -120.f), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (ImGui::BeginTable(title.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable |
			ImGuiTableFlags_RowBg, ImVec2(0.f, 500.f)))
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
					strcpy_s(editor.bufferKey, sizeof(editor.bufferKey), key.c_str());
					strcpy_s(editor.bufferValue, sizeof(editor.bufferValue), value.c_str());
				}
				ImGui::TableSetColumnIndex(1);
				ImGui::Text(value.c_str());
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();
	}

	if (editor.isAdding || editor.isEditing)
	{
		drawFlatIniOperationButtons(editor);
	}
}

/**
 * @brief 
 * @param editor reference to the editor
 * @param isNewRecord if we add a new record or edit existent
 */
void EditorScreen::drawFlatIniOperationButtons(FlatIniEditor& editor)
{
	ImGui::Separator();
	if (editor.isAdding)
	{
		ImGui::Text("Add new record");
	}
	else
	{
		ImGui::Text("Edit selected record:");
	}
	ImGui::InputText("Key ID", editor.bufferKey, sizeof(editor.bufferKey));
	ImGui::InputText("Value data", editor.bufferValue, sizeof(editor.bufferValue));
	std::string keyToCheck = editor.bufferKey;

	// Save or cancel changes. Block if there is a key duplication
	bool isDuplicate{ false };
	bool isEmpty = keyToCheck == "";
	if (editor.isAdding)
	{
		isDuplicate = isKeyDuplicate(keyToCheck, editor);
	}
	if (editor.isEditing)
	{
		isDuplicate = (isKeyDuplicate(keyToCheck, editor) && keyToCheck != editor.selectedKey);
	}
	ImGui::BeginDisabled(isDuplicate || isEmpty);
	if (ImGui::Button("Apply"))
	{
		editor.data[editor.bufferKey] = editor.bufferValue;
		if (editor.isEditing && editor.bufferKey != editor.selectedKey)
		{
			editor.data.erase(editor.selectedKey);
		}
		resetEditorState(editor);
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
	{
		resetEditorState(editor);
	}
	ImGui::EndDisabled();

	if (isDuplicate)
	{
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 0.8f), "Key is already exist. Enter the new one");
	}
	if (isEmpty)
	{
		ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 0.8f), "Key can't be an empty string");
	}
}

/**
 * @brief Check if the new key of the record duplicate another key in the editor
 * @return if the editor.data contain parameter key
 */
bool EditorScreen::isKeyDuplicate(const std::string& key, FlatIniEditor& editor) const
{
	return editor.data.contains(key);
}

/**
 * @brief Resets the temporary interaction states and input buffers of the flat editor.
 *
 * Clears the raw character buffers for key-value entry by injecting null-terminators
 * and toggles action flags (isAdding, isEditing) back to false. This ensures cached
 * data from the previous operation does not leak into the next interface frame.
 *
 * @param editor A reference to the FlatIniEditor instance being manipulated.
 */
void EditorScreen::resetEditorState(FlatIniEditor& editor)
{
	editor.isAdding = false;
	editor.isEditing = false;
	editor.bufferKey[0] = '\0';
	editor.bufferValue[0] = '\0';
	editor.selectedKey = "";
}


void EditorScreen::saveFlatIniEditor(FlatIniEditor& editor)
{
}
