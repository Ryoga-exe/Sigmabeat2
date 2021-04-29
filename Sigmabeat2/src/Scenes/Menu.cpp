#include "Menu.hpp"

Menu::Menu(const InitData& init)
    : IScene(init) {

    m_musicNum = 6;
    m_activeIndex = 0;
    m_state = 0;
}

void Menu::update() {

    if (KeyEscape.down()) {
        changeScene(SceneState::Setup);
    }

    if (KeyRight.down()) {
        m_activeIndex++;
    }
    
    if (KeyLeft.down()) {
        m_activeIndex--;
    }

    if (Key1.pressed()) {
        m_state = Min(1.0, m_state + 0.008);
    }
    if (Key2.pressed()) {
        m_state = Max(-1.0, m_state - 0.008);
    }
    if (KeyUp.pressed()) {
        m_state = 1.0;
    }
    if (KeyDown.pressed()) {
        m_state = -1.0;
    }
    if (KeyR.pressed()) {
        m_state = 0.0;
    }
    // tileOffsetX = Math::SmoothDamp(tileOffsetX, targetTileOffsetX, tileOffsetXVelocity, 0.1, Scene::DeltaTime());

}


namespace UI {
    constexpr double ActiveIndexSize = 250;
    constexpr double NormalIndexSize = 200;
    constexpr double MarginSize = 50;
}

void Menu::draw() const {

    ClearPrint();

    Print << m_activeIndex << U" / " << m_musicNum-1;
    Print << m_state;

    const double
        activeSize = 250 - (250 - 200) * Abs(m_state),
        normalSize = 200,
        marginSize = normalSize / 4.0;
    auto [w, h] = Scene::Center();

    double activeX = w - (250 / 2 + marginSize + 200 / 2) * m_state;
    
    const double baseY = h + normalSize / 2;

    // drawActiveIndex
    RectF(Arg::bottomCenter = Vec2{ activeX, baseY }, activeSize).draw(Palette::Mediumpurple);

    double x = activeX + activeSize / 2 + marginSize;

    for (int32 index = m_activeIndex + 1; index < m_musicNum; index++) {
        // もし、完全に画面外ならbreak
        if (false) {
            break;
        }

        if (index == m_activeIndex + 1) {
            RectF(Arg::bottomLeft = Vec2{ x, baseY }, normalSize + (250 - 200) * Max(0.0, m_state)).draw(Palette::Purple);
            x += (250 - 200) * Max(0.0, m_state);
        }
        else {
            RectF(Arg::bottomLeft = Vec2{ x, baseY }, normalSize).draw(Palette::Purple);
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
            RectF(Arg::bottomRight = Vec2{ x, baseY }, normalSize - (250 - 200) * Min(0.0, m_state)).draw(Palette::Purple);
            x += (250 - 200) * Min(0.0, m_state);
        }
        else {
            RectF(Arg::bottomRight = Vec2{ x, baseY }, normalSize).draw(Palette::Purple);
        }

        x -= marginSize + normalSize;
    }

    RectF(Arg::bottomCenter = Vec2{ w - 250 / 2 - 100 - 50, baseY }, normalSize).draw(ColorF(0.0, 0.5));
    RectF(Arg::bottomCenter = Vec2{ w + 250 / 2 + 100 + 50, baseY }, normalSize).draw(ColorF(0.0, 0.5));
}