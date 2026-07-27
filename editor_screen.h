#pragma once
#include "asset_palette.h"
#include "game_data.h"
#include <disreality_engine.h>
#include <memory>

/**
 * @brief 
 */
class EditorScreen : public dr::Screen
{
public:
	EditorScreen();
	~EditorScreen();
	void init() override;
	void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
private:
	struct ScreenInputVisitor;
	// Flat (structure) ini editors as strings, pathes to textures
	struct FlatIniEditor;
	std::unique_ptr<FlatIniEditor> mStringEditor;
	void drawFlatIniEditor(const std::string& title, FlatIniEditor& editor);
	void drawFlatIniOperationButtons(FlatIniEditor& editor);
	bool isKeyDuplicate(const std::string& key, FlatIniEditor& editor) const;
	void resetEditorState(FlatIniEditor& editor);
	void saveFlatIniEditor(FlatIniEditor& editor) const;

	// Visual TilemapEditor
	struct TilemapEditor;
	std::unique_ptr<TilemapEditor> mTilemapEditor;
	bool mShowTilemapEditor{ false };
	void drawTilemapEditor(float dt);
	AssetPalette mFloorAsset;
	bool mMapIsReady{ false };

	sf::View mMainView{ sf::FloatRect({0, 0}, {gd::GraphicsResolution.x, gd::GraphicsResolution.y}) };
	sf::View mTilemapView;

	bool mExitEditor{ false };
	bool mShowStringEditor{ false };

	std::string mProjectPath{};
	bool mShowOpenProjectModalWindow{ false };
	char mProjectPathBuffer[512]{""};

	dr::CursorComponent mCursor{mMainView};
};