﻿#include "Menu.hpp"

Menu::Menu(const InitData& init)
    : IScene(init), m_tileOffsetXVelocity(0.0), m_animateState(0.0), m_tileOffsetStopwatch(true) {

    m_selectedTileSize = UI::Menu::TileSize;
    m_level = 3;

    m_selectedTileX = Scene::CenterF().x;

    m_indexSize = static_cast<int32>(m_scores.getScoreNum());
    m_selectedIndex = 0;

    updateTiles();

    if (AudioAsset::IsRegistered(U"Menu.demo")) AudioAsset::Unregister(U"Menu.demo");
    AudioAsset::Register(U"Menu.demo", m_scores.getMusicPath(m_selectedIndex), AssetParameter::LoadAsync());
    AudioAsset(U"Menu.demo").setPosSec(m_scores.getDemoStartMs(m_selectedIndex) / 1000.0);
}

void Menu::update() {

    /**/

    if (AudioAsset::IsReady(U"Menu.demo") && !AudioAsset(U"Menu.demo").isPlaying()) {
        if (AudioAsset(U"Menu.demo")) {
            m_stopwatch.restart();
            AudioAsset(U"Menu.demo").setVolume(0.0);
            AudioAsset(U"Menu.demo").setPosSec(m_scores.getDemoStartMs(m_selectedIndex) / 1000.0);
            AudioAsset(U"Menu.demo").play();
        }
    }

    if (m_stopwatch.isRunning() && AudioAsset(U"Menu.demo")) {
        constexpr int32 introMs = 500;
        constexpr int32 length = 24800;
        constexpr int32 span = 200;

        int32 elapsedMs = m_stopwatch.ms();

        if (elapsedMs < introMs) {
            AudioAsset(U"Menu.demo").setVolume((double)elapsedMs / introMs);
        }
        else if (elapsedMs < length - introMs) {
            AudioAsset(U"Menu.demo").setVolume(1.0);
        }
        else if (elapsedMs < length) {
            AudioAsset(U"Menu.demo").setVolume((double)(length - elapsedMs) / introMs);
        }
        else if (elapsedMs > length + span) {
            m_stopwatch.reset();
            AudioAsset(U"Menu.demo").stop();
        }
    }

    /**/

    //
    // ToDo: キー長押しの対応
    // ToDo: Registerしてる最中にUnregisterすると待ちが発生する？ので直す
    // ↑ 先に曲idごとに全部Registerして、矢印キーが押されたらstop -> Unregisterするキューに追加する
    // -> もし、IsReady() == true になったら(もうなってたら)Unregister
    //

    if (KeyEscape.down()) {
        AudioAsset(U"Menu.demo").stop(0.5s);
        
        changeScene(SceneState::Setup);
    }

    if (m_selectedIndex + 1 < m_indexSize && KeyRight.down()) {
        m_animateState = -1.0;
        m_selectedIndex++;

        m_tileOffsetStopwatch.restart();

        /**/
        m_stopwatch.reset();
        AudioAsset(U"Menu.demo").stop();

        AudioAsset::Unregister(U"Menu.demo");
        AudioAsset::Register(U"Menu.demo", m_scores.getMusicPath(m_selectedIndex), AssetParameter::LoadAsync());

        /**/
    }
    
    if (m_selectedIndex > 0 && KeyLeft.down()) {
        m_animateState = 1.0;
        m_selectedIndex--;

        m_tileOffsetStopwatch.restart();

        /**/
        m_stopwatch.reset();

        AudioAsset(U"Menu.demo").stop();

        AudioAsset::Unregister(U"Menu.demo");
        AudioAsset::Register(U"Menu.demo", m_scores.getMusicPath(m_selectedIndex), AssetParameter::LoadAsync());

        /**/
    }

    if (m_animateState != 0.00) {
        m_animateState = Math::SmoothDamp(m_animateState, 0.0, m_tileOffsetXVelocity, 0.1, Scene::DeltaTime());
    }

    if (KeyUp.down())   m_level = (m_level + 1) % Score::LevelNum;
    if (KeyDown.down()) m_level = (m_level + Score::LevelNum - 1) % Score::LevelNum;

    updateTiles();

}

void Menu::draw() const {

    if (m_indexSize == 0) return;

    RectF(Arg::bottomCenter = Vec2{ Scene::CenterF().x, m_tileBaseY }, Scene::Width(), UI::Menu::NormalTileSize.y).stretched(50).draw(ColorF(0, 0.5));
    
    Quad(Vec2(Scene::CenterF().x - UI::Menu::TileSize.x / 2.0 - UI::Menu::TileMargin - UI::Menu::SelectedTileMarginSize, m_tileBaseY - UI::Menu::NormalTileSize.y - 50)
        ,Vec2(Scene::CenterF().x + UI::Menu::TileSize.x / 2.0 + UI::Menu::TileMargin + UI::Menu::SelectedTileMarginSize, m_tileBaseY - UI::Menu::NormalTileSize.y - 50)
        ,Vec2(Scene::CenterF().x + UI::Menu::TileSize.x / 2.0, m_tileBaseY - UI::Menu::NormalTileSize.y - 200)
        ,Vec2(Scene::CenterF().x - UI::Menu::TileSize.x / 2.0, m_tileBaseY - UI::Menu::NormalTileSize.y - 200)).draw(ColorF(0, 0.5));

    drawTiles();

}

void Menu::updateTiles() {
    auto [centerX, centerY] = Scene::CenterF();
    m_selectedTileSize = UI::Menu::TileSize - (UI::Menu::TileSize * 0.3) * Abs(m_animateState);
    m_tileBaseY = centerY + UI::Menu::TileSize.y / 2;
    m_selectedTileX = centerX - (UI::Menu::TileSize.x / 2.0 + UI::Menu::NormalTileSize.x / 2.0 + UI::Menu::TileMargin + UI::Menu::SelectedTileMarginSize) * m_animateState;
}

void Menu::drawTiles() const {

    // drawSelectedIndex
    RectF selectedTile(Arg::bottomCenter = Vec2{ m_selectedTileX, m_tileBaseY}, m_selectedTileSize);
    selectedTile.drawShadow({ 0.0, 0.0 }, 25, 15.0, ColorF(Palette::Gold, 0.5 + Periodic::Sine0_1(2.0s) * 0.5));
    selectedTile(m_tile.get(m_selectedIndex, Score::LevelColor[m_level], Max(0.0, m_tileOffsetStopwatch.sF() - 1.3))).draw();

    double x = m_selectedTileX + m_selectedTileSize.x / 2.0 + UI::Menu::TileMargin + UI::Menu::SelectedTileMarginSize * (m_animateState >= 0.0 ? 1.0 : 1.0 + m_animateState);

    for (int32 index = m_selectedIndex + 1; index < m_indexSize; index++) {
        
        if (Scene::Width() < x) break;    // 画面外

        RectF tile(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, UI::Menu::NormalTileSize);

        if (index == m_selectedIndex + 1) {
            tile.set(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, UI::Menu::NormalTileSize + (UI::Menu::TileSize * 0.3) * Max(0.0, m_animateState));
            x += ((UI::Menu::TileSize.x * 0.3) + UI::Menu::SelectedTileMarginSize) * Max(0.0, m_animateState);
        }

        tile(m_tile.get(index, Score::LevelColor[m_level])).draw();

        x += UI::Menu::TileMargin + UI::Menu::NormalTileSize.x;
    }

    x = m_selectedTileX - m_selectedTileSize.x / 2.0 - UI::Menu::TileMargin - UI::Menu::SelectedTileMarginSize * (m_animateState <= 0.0 ? 1.0 : 1.0 - m_animateState);

    for (int32 index = m_selectedIndex - 1; index >= 0; index--) {
        
        if (x < 0) break;    // 画面外

        RectF tile(Arg::bottomRight = Vec2{ x, m_tileBaseY }, UI::Menu::NormalTileSize);

        if (index == m_selectedIndex - 1) {
            tile.set(Arg::bottomRight = Vec2{ x, m_tileBaseY }, UI::Menu::NormalTileSize - (UI::Menu::TileSize * 0.3) * Min(0.0, m_animateState));
            x += ((UI::Menu::TileSize.x * 0.3) + UI::Menu::SelectedTileMarginSize) * Min(0.0, m_animateState);
        }

        tile(m_tile.get(index, Score::LevelColor[m_level])).draw();

        x -= UI::Menu::TileMargin + UI::Menu::NormalTileSize.x;
    }
}