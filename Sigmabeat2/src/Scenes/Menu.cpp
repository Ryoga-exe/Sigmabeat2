#include "Menu.hpp"

Menu::Menu(const InitData& init)
    : IScene(init), m_tileOffsetXVelocity(0.0), m_animateState(0.0), m_tileOffsetStopwatch(true), m_tileState(0.0), m_audition(0) {

    m_level = 3;

    m_scaleRate = Scene::Height() / 900.0;
    m_indexSize = static_cast<int32>(m_scores.getScoreNum());
    m_index = 0;

    updateTiles();
}

void Menu::update() {

    /// test

    if (m_tileState == 0.00 && Abs(m_animateState) <= 0.05 && (KeyEnter | KeySpace).down()) {
        m_stopwatch.restart();
    }

    m_tileState = EaseOutCubic(Min(m_stopwatch.sF() * 2, 1.0));

    ///



    m_audition.update(m_index);
    m_audition.autoPlayAndStop();

    if (KeyEscape.down()) {
        m_audition.stop(0.5s);
        
        changeScene(SceneState::Setup);
    }

    if (m_index + 1 < m_indexSize && KeyRight.down()) {
        m_animateState = -1.0;
        m_index++;

        m_tileOffsetStopwatch.restart();
    }
    
    if (m_index > 0 && KeyLeft.down()) {
        m_animateState = 1.0;
        m_index--;

        m_tileOffsetStopwatch.restart();
    }

    if (m_animateState != 0.00) {
        m_animateState = Math::SmoothDamp(m_animateState, 0.0, m_tileOffsetXVelocity, 0.1, Scene::DeltaTime());
    }

    if (KeyUp.down())   m_level = (m_level + 1) % Score::LevelNum;
    if (KeyDown.down()) m_level = (m_level + Score::LevelNum - 1) % Score::LevelNum;


    m_scaleRate = Scene::Height() / 900.0;
    updateTiles();

}

void Menu::draw() const {

    if (m_indexSize == 0) return;

    const double bgOffset = 30 * m_scaleRate;

    RectF(Arg::bottomCenter = Vec2{ Scene::CenterF().x, m_tileBaseY - bgOffset }, Scene::Width(), m_normalTileSize.y).stretched(80 * m_scaleRate).draw(ColorF(0, 0.5));
    
    Quad(Vec2(Scene::CenterF().x - m_tileSize.x / 2.0 - m_tileMargin - m_selectedTileMargin, m_tileBaseY - m_normalTileSize.y - 80 * m_scaleRate)
        ,Vec2(Scene::CenterF().x + m_tileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin, m_tileBaseY - m_normalTileSize.y - 80 * m_scaleRate)
        ,Vec2(Scene::CenterF().x + m_tileSize.x / 2.0, m_tileBaseY - m_normalTileSize.y - 200 * m_scaleRate)
        ,Vec2(Scene::CenterF().x - m_tileSize.x / 2.0, m_tileBaseY - m_normalTileSize.y - 200 * m_scaleRate)).movedBy(0, -bgOffset).draw(ColorF(0, 0.5));

    FontAsset(U"Tile.detail")(U"{} / {}"_fmt(m_index + 1, m_indexSize)).draw(Arg::bottomCenter = Vec2{ Scene::CenterF().x, m_tileBaseY + 45 * m_scaleRate }, ColorF(0.8));
    drawTiles();

}

void Menu::updateTiles() {
    auto [centerX, centerY] = Scene::CenterF();

    m_tileSize = SizeF(UI::Menu::TileSize) * m_scaleRate;
    m_tileMargin = UI::Menu::TileMargin * m_scaleRate;
    m_selectedTileMargin = UI::Menu::SelectedTileMargin * m_scaleRate;

    m_selectedTileSize = m_tileSize - (m_tileSize * 0.3) * Abs(m_animateState);
    m_tileBaseY = centerY + m_tileSize.y / 2;
    m_normalTileSize = m_tileSize * 0.7;
    m_selectedTileX = centerX - (m_tileSize.x / 2.0 + m_normalTileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin) * m_animateState;
}

void Menu::drawTiles() const {

    double x = m_selectedTileX + (m_selectedTileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin * (m_animateState >= 0.0 ? 1.0 : 1.0 + m_animateState)) * (1.0 + m_tileState);

    for (int32 index = m_index + 1; index < m_indexSize; index++) {
        
        if (Scene::Width() < x - m_normalTileSize.x * m_tileState) break;    // 画面外

        RectF tile(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_normalTileSize);

        if (index == m_index + 1) {
            tile.set(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_normalTileSize + (m_tileSize * 0.3) * Max(0.0, m_animateState));
            x += ((m_tileSize.x * 0.3) + m_selectedTileMargin) * Max(0.0, m_animateState);
        }

        tile.scaled(1.0 + m_tileState)(m_tile.get(index, Score::LevelColor[m_level])).draw(ColorF(1.0, 1.0 - Abs(m_tileState)));

        x += (m_tileMargin + m_normalTileSize.x) * (1.0 + m_tileState);
    }


    x = m_selectedTileX - (m_selectedTileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin * (m_animateState <= 0.0 ? 1.0 : 1.0 - m_animateState)) * (1.0 + m_tileState);

    for (int32 index = m_index - 1; index >= 0; index--) {
        
        if (x + m_normalTileSize.x * m_tileState < 0) break;    // 画面外

        RectF tile(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_normalTileSize);

        if (index == m_index - 1) {
            tile.set(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_normalTileSize - (m_tileSize * 0.3) * Min(0.0, m_animateState));
            x += ((m_tileSize.x * 0.3) + m_selectedTileMargin) * Min(0.0, m_animateState);
        }

        tile.scaled(1.0 + m_tileState)(m_tile.get(index, Score::LevelColor[m_level])).draw(ColorF(1.0, 1.0 - Abs(m_tileState)));

        x -= (m_tileMargin + m_normalTileSize.x) * (1.0 + m_tileState);
    }

    // drawSelectedIndex
    RectF selectedTile(Arg::bottomCenter = Vec2{ m_selectedTileX, m_tileBaseY }, m_selectedTileSize);
    selectedTile.drawShadow({ 0.0, 0.0 }, 25, 15.0, ColorF(Palette::Gold, 0.5 + Periodic::Sine0_1(2.0s) * 0.5));
    selectedTile(m_tile.get(m_index, Score::LevelColor[m_level], Max(0.0, m_tileOffsetStopwatch.sF() - 1.0))).draw();

}