#include "Menu.hpp"

Menu::Menu(const InitData& init)
    : IScene(init), m_tileOffsetXVelocity(0.0) {

    m_musicNum = 6;
    m_activeIndex = 0;
    m_state = 0;
}

void Menu::update() {

    if (KeyEscape.down()) {
        changeScene(SceneState::Setup);
    }

    if (m_activeIndex + 1 < m_musicNum && KeyRight.down()) {
        m_state = -1.0;
        m_activeIndex++;
    }
    
    if (m_activeIndex > 0 && KeyLeft.down()) {
        m_state = 1.0;
        m_activeIndex--;
    }

    if (m_state != 0.00) {
        m_state = Math::SmoothDamp(m_state, 0.0, m_tileOffsetXVelocity, 0.1, Scene::DeltaTime());
    }
}


namespace UI {
    constexpr double ActiveIndexSize = 250;
    constexpr double NormalIndexSize = 200;
    constexpr double SizeBetween = ActiveIndexSize - NormalIndexSize;
    constexpr double MarginSize = NormalIndexSize / 4.0;
}

void Menu::draw() const {

    ClearPrint();

    Print << m_activeIndex << U" / " << m_musicNum-1;
    Print << m_state;
    Print << m_tileOffsetXVelocity;

    const double
        activeSize = UI::ActiveIndexSize - UI::SizeBetween * Abs(m_state),
        normalSize = UI::NormalIndexSize,
        marginSize = UI::MarginSize;
    auto [w, h] = Scene::Center();

    double activeX = w - (UI::ActiveIndexSize / 2 + UI::NormalIndexSize / 2 + marginSize) * m_state;
    
    const double baseY = h + normalSize / 2;

    // drawActiveIndex
    RectF(Arg::bottomCenter = Vec2{ activeX, baseY }, activeSize).draw(HueToColor(60 * m_activeIndex));

    double x = activeX + activeSize / 2 + marginSize;

    for (int32 index = m_activeIndex + 1; index < m_musicNum; index++) {
        // もし、完全に画面外ならbreak
        if (false) {
            break;
        }

        if (index == m_activeIndex + 1) {
            RectF(Arg::bottomLeft = Vec2{ x, baseY }, normalSize + UI::SizeBetween * Max(0.0, m_state)).draw(HueToColor(60 * index));
            x += UI::SizeBetween * Max(0.0, m_state);
        }
        else {
            RectF(Arg::bottomLeft = Vec2{ x, baseY }, normalSize).draw(HueToColor(60 * index));
        }

        x += marginSize + normalSize;
    }

    x = activeX - activeSize / 2 - marginSize;

    for (int32 index = m_activeIndex - 1; index >= 0; index--) {
        // もし、完全に画面外ならbreak
        if (false) {
            break;
        }
        if (index == m_activeIndex - 1) {
            RectF(Arg::bottomRight = Vec2{ x, baseY }, normalSize - UI::SizeBetween * Min(0.0, m_state)).draw(HueToColor(60 * index));
            x += UI::SizeBetween * Min(0.0, m_state);
        }
        else {
            RectF(Arg::bottomRight = Vec2{ x, baseY }, normalSize).draw(HueToColor(60 * index));
        }

        x -= marginSize + normalSize;
    }

}