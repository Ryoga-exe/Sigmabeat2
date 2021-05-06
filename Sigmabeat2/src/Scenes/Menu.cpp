#include "Menu.hpp"

namespace UI {
    constexpr double SelectedTileSize = 250;
    constexpr double NormalIndexSize = 200;
    constexpr double SizeBetween = SelectedTileSize - NormalIndexSize;
    constexpr double MarginSize = NormalIndexSize / 4.0;
}

Menu::Menu(const InitData& init)
    : IScene(init), m_tileOffsetXVelocity(0.0), m_animateState(0.0) {
    
    m_selectedTileSize = UI::SelectedTileSize;
    m_tileSize = UI::NormalIndexSize;
    m_tileMargin = UI::MarginSize;
    m_selectedTileX = Scene::CenterF().x;

    m_indexSize = static_cast<int32>(m_scores.getScoreSize());
    m_selectedIndex = 0;

    updateTiles();
}

void Menu::update() {

    if (KeyEscape.down()) {
        changeScene(SceneState::Setup);
    }

    if (m_selectedIndex + 1 < m_indexSize && KeyRight.down()) {
        m_animateState = -1.0;
        m_selectedIndex++;
    }
    
    if (m_selectedIndex > 0 && KeyLeft.down()) {
        m_animateState = 1.0;
        m_selectedIndex--;
    }

    if (m_animateState != 0.00) {
        m_animateState = Math::SmoothDamp(m_animateState, 0.0, m_tileOffsetXVelocity, 0.1, Scene::DeltaTime());
    }

    updateTiles();

}

void Menu::draw() const {

    if (m_indexSize == 0) return;

    drawTiles();

}

void Menu::updateTiles() {
    auto [centerX, centerY] = Scene::CenterF();
    m_selectedTileSize = UI::SelectedTileSize - UI::SizeBetween * Abs(m_animateState);
    m_tileBaseY = centerY + m_tileSize / 2;
    m_selectedTileX = centerX - (UI::SelectedTileSize / 2 + UI::NormalIndexSize / 2 + m_tileMargin) * m_animateState;
}

void Menu::drawTiles() const {

    // drawSelectedIndex

    RectF(Arg::bottomCenter = Vec2{ m_selectedTileX, m_tileBaseY }, m_selectedTileSize)(m_scores.getTexture(m_selectedIndex)).draw();

    double x = m_selectedTileX + m_selectedTileSize / 2 + m_tileMargin;

    for (int32 index = m_selectedIndex + 1; index < m_indexSize; index++) {
        // もし画面外ならbreak
        if (Scene::Width() < x) break;

        RectF tile(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_tileSize);

        if (index == m_selectedIndex + 1) {
            tile.set(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_tileSize + UI::SizeBetween * Max(0.0, m_animateState));
            x += UI::SizeBetween * Max(0.0, m_animateState);
        }

        tile(m_scores.getTexture(index)).draw();

        x += m_tileMargin + m_tileSize;
    }

    x = m_selectedTileX - m_selectedTileSize / 2 - m_tileMargin;

    for (int32 index = m_selectedIndex - 1; index >= 0; index--) {
        // もし画面外ならbreak
        if (x < 0) break;

        RectF tile(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_tileSize);

        if (index == m_selectedIndex - 1) {
            tile.set(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_tileSize - UI::SizeBetween * Min(0.0, m_animateState));
            x += UI::SizeBetween * Min(0.0, m_animateState);
        }

        tile(m_scores.getTexture(index)).draw();

        x -= m_tileMargin + m_tileSize;
    }
}