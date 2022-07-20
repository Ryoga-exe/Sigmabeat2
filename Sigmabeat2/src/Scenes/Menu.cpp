#include "Menu.hpp"
#include "Config/Config.hpp"
#include "Asset/Asset.hpp"
#include "UI/Color.hpp"

Menu::Menu(const InitData& init)
    : IScene(init), m_tileOffsetXVelocity(0.0), m_animateState(0.0)
    , m_backgroundTexture(Asset::Image::Polygon3)
    , m_tileOffsetStopwatch(StartImmediately::Yes), m_tileState(0.0), m_audition(0), m_menuState(0) {

    Scene::SetBackground(UI::MenuBackgroundColor);
    if (!Config::Get().keepSceneSize) {
        Scene::SetLetterbox(UI::MenuBackgroundColor);
    }

    m_settingState = 0;

    m_level = getData().selectLevel;;

    m_indexSize = static_cast<int32>(m_scores.size());
    m_index = getData().selectIndex;
    m_settingIndex = 1;

    updateScaleRate();
    updateTiles();

    AudioAsset::Load(U"SE.move");
    AudioAsset::Load(U"SE.levelChange");

    m_sleepTimer.restart();

}

Menu::~Menu() {
    
}

void Menu::update() {

    if (!Keyboard::GetAllInputs().empty()) {
        m_sleepTimer.restart();
    }

    getData().selectIndex = m_index;
    getData().selectLevel = m_level;

    if (m_sleepTimer.elapsed() >= 1.5min) {
        m_audition.stop(0.5s);

        changeScene(SceneState::Setup, 1.0s);
    }

    if (m_settingState == 0) {
        if (KeyEscape.down()) {
            m_audition.stop(0.5s);

            changeScene(SceneState::Setup, 1.0s);
        }

        if (m_tileState == 0.0) {
            if (m_index + 1 < m_indexSize && (KeyRight | KeyL).down()) {
                m_animateState = -1.0;
                m_index++;

                m_tileOffsetStopwatch.restart();
                AudioAsset(U"SE.move").playOneShot();
            }

            if (m_index > 0 && (KeyLeft | KeyS).down()) {
                m_animateState = 1.0;
                m_index--;

                m_tileOffsetStopwatch.restart();
                AudioAsset(U"SE.move").playOneShot();
            }

            if (Abs(m_animateState) <= 0.05 && (KeyEnter | KeySpace | KeyF | KeyJ).down()) {
                m_settingIndex = 1;
                m_stopwatch.restart();
            }
        }

        m_tileState = EaseOutQuart(Min(m_stopwatch.sF() * 2.3, 1.0));

        if (m_tileState == 1.0) {
            m_settingState = 1;
            m_settingIndex = 1;
            m_stopwatch.reset();
        }
    }
    else if (m_settingState == 1) {
        if (m_tileState == 1.0) {
            if (Abs(m_animateState) <= 0.05 ) {
                if (KeyEscape.down() || KeyD.pressedDuration() >= 1.0s) {
                    m_settingIndex = 1;
                    m_stopwatch.restart();
                }
                if ((KeyEnter | KeySpace | KeyF | KeyJ).down() && SettingTiles[m_settingIndex] == U"MUSIC") {
                    if (m_scores.get(m_index).level[m_level] > 0) {
                        m_settingState = 2;
                        m_stopwatch.restart();

                        m_audition.stop(3.0s);
                    }
                }
            }


            if (m_settingIndex + 1 < SettingTiles.size() && (KeyRight | KeyL).down()) {
                m_animateState = -1.0;
                m_settingIndex++;

                m_tileOffsetStopwatch.restart();
                AudioAsset(U"SE.move").playOneShot();
            }

            if (m_settingIndex > 0 && (KeyLeft | KeyS).down()) {
                m_animateState = 1.0;
                m_settingIndex--;

                m_tileOffsetStopwatch.restart();
                AudioAsset(U"SE.move").playOneShot();
            }
        }

        m_tileState = 1.0 - EaseOutQuart(Min(m_stopwatch.sF() * 2.3, 1.0));


        if (m_tileState == 0.0) {
            m_settingState = 0;
            m_stopwatch.reset();
        }
    }
    else {
        m_tileState = 1.0 + EaseOutQuart(Min(m_stopwatch.sF() * 2.3, 1.0));

        if (m_tileState == 2.0 && m_stopwatch.elapsed() >= 2.5s) {
            m_stopwatch.pause();
            changeScene(SceneState::Game);
        }
    }

    m_animateState = Math::SmoothDamp(m_animateState, 0.0, m_tileOffsetXVelocity, 0.1);

    if (m_settingState != 2) {

        m_audition.update(m_index);
        m_audition.autoPlayAndStop();

        if (SettingTiles[m_settingIndex] == U"MUSIC") {
            if ((KeyUp | KeyK).down()) {
                m_level = (m_level + 1) % Score::LevelNum;
                AudioAsset(U"SE.levelChange").playOneShot();
            }
            if ((KeyDown | KeyD).down()) {
                m_level = (m_level + Score::LevelNum - 1) % Score::LevelNum;
                AudioAsset(U"SE.levelChange").playOneShot();
            }
        }
        else {
            auto& target = getData().setting[SettingTiles[m_settingIndex]];
            if ((KeyUp | KeyK).down() && target.value + 1 <= target.maxValue) {
                target.value++;
            }
            if ((KeyDown | KeyD).down() && target.value - 1 >= target.minValue) {
                target.value--;
            }
        }
    }

    updateScaleRate();
    updateTiles();

}

void Menu::draw() const {

    if (m_indexSize == 0) return;

    drawBackground();
    drawUserPlate();

    drawSettingTiles();
    drawTiles();
    drawSelectedTile();

    //RectF{ 0, 0, Scene::Width(), 80 }.draw(ColorF(0, 0, 0, 0.5));
    //FontAsset(U"Menu")(U"MUSIC SELECT").draw(Arg::topLeft(10.0, 5.0), Palette::White);

}

void Menu::updateScaleRate() {
    m_scaleRate = Scene::Height() / 900.0;
}

void Menu::updateTiles() {
    auto [centerX, centerY] = Scene::CenterF();

    m_tileSize = SizeF(UI::TileSize) * m_scaleRate;
    m_tileMargin = UI::TileMargin * m_scaleRate;
    m_selectedTileMargin = UI::SelectedTileMargin * m_scaleRate;

    m_selectedTileSize = m_tileSize - (m_tileSize * 0.3) * Abs(m_animateState);
    m_tileBaseY = centerY + m_tileSize.y / 2;
    m_normalTileSize = m_tileSize * 0.7;
    m_selectedTileX = centerX - (m_tileSize.x / 2.0 + m_normalTileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin) * m_animateState;
}

void Menu::drawBackground() const {
    const double bgOffset = 30 * m_scaleRate;

    double r = abs(sin(Scene::Time() * 0.3));
    const ColorF colors[4] = {
        ColorF(0.0, 0.0, r, 0.20),
        ColorF(0.0, 1.0, r, 0.20),
        ColorF(1.0, 1.0, r, 0.20),
        ColorF(1.0, 0.0, r, 0.20),
    };
    RectF{ Scene::Size() }.draw(colors);
    RectF{ Scene::Size() }(m_backgroundTexture).draw(ColorF(1.0, 1.0, 1.0, 0.15));

    RectF{ 20, 20, 7, 80 }.draw(Palette::White);
    FontAsset(U"Game.score")(U"MUSIC SELECT").draw(40, 30);

    RectF(Arg::bottomCenter = Vec2{ Scene::CenterF().x, m_tileBaseY - bgOffset }, Scene::Width(), m_normalTileSize.y).stretched(80 * m_scaleRate).draw(ColorF(0, 0.5));

    Quad(Vec2(Scene::CenterF().x - m_tileSize.x / 2.0 - m_tileMargin - m_selectedTileMargin, m_tileBaseY - m_normalTileSize.y - 80 * m_scaleRate)
        ,Vec2(Scene::CenterF().x + m_tileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin, m_tileBaseY - m_normalTileSize.y - 80 * m_scaleRate)
        ,Vec2(Scene::CenterF().x + m_tileSize.x / 2.0, m_tileBaseY - m_normalTileSize.y - 200 * m_scaleRate)
        ,Vec2(Scene::CenterF().x - m_tileSize.x / 2.0, m_tileBaseY - m_normalTileSize.y - 200 * m_scaleRate)).movedBy(0, -bgOffset).draw(ColorF(0, 0.5));

    FontAsset(U"Tile.detail")(U"/").draw(Arg::bottomCenter = Vec2{ Scene::CenterF().x, m_tileBaseY + 45 * m_scaleRate }, ColorF(0.8));
    FontAsset(U"Tile.detail")(U"  {}"_fmt(m_indexSize)).draw(Arg::bottomLeft = Vec2{ Scene::CenterF().x, m_tileBaseY + 45 * m_scaleRate }, ColorF(0.8));
    FontAsset(U"Tile.detail")(U"{}  "_fmt(m_index + 1)).draw(Arg::bottomRight = Vec2{ Scene::CenterF().x, m_tileBaseY + 45 * m_scaleRate }, ColorF(1.0));
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

        tile.scaled(1.0 + m_tileState)(m_tile.get(index, m_level)).draw(ColorF(1.0, 1.0 - Abs(m_tileState)));

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

        tile.scaled(1.0 + m_tileState)(m_tile.get(index, m_level)).draw(ColorF(1.0, 1.0 - Abs(m_tileState)));

        x -= (m_tileMargin + m_normalTileSize.x) * (1.0 + m_tileState);
    }
}

void Menu::drawSettingTiles() const {

    double x = m_selectedTileX + (m_selectedTileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin * (m_animateState >= 0.0 ? 1.0 : 1.0 + m_animateState)) * (m_tileState);

    for (int32 index = m_settingIndex + 1; index < SettingTiles.size(); index++) {

        if (Scene::Width() < x - m_normalTileSize.x * (m_tileState)) break;    // 画面外

        RectF tile(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_normalTileSize);

        if (index == m_settingIndex + 1) {
            tile.set(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_normalTileSize + (m_tileSize * 0.3) * Max(0.0, m_animateState));
            x += ((m_tileSize.x * 0.3) + m_selectedTileMargin) * Max(0.0, m_animateState);
        }

        if (SettingTiles[index] == U"MUSIC") {
            tile.scaled((m_tileState))(m_tile.get(m_index, m_level)).draw(ColorF(1.0, 1.0 - Abs(m_tileState - 1.0)));
        }
        else {
            tile.scaled((m_tileState))(m_tile.get(SettingTiles[index], getData().setting[SettingTiles[index]], Palette::Slategray)).draw(ColorF(1.0, 1.0 - Abs(m_tileState - 1.0)));
        }

        x += (m_tileMargin + m_normalTileSize.x) * (m_tileState);
    }


    x = m_selectedTileX - (m_selectedTileSize.x / 2.0 + m_tileMargin + m_selectedTileMargin * (m_animateState <= 0.0 ? 1.0 : 1.0 - m_animateState)) * (m_tileState);

    for (int32 index = m_settingIndex - 1; index >= 0; index--) {

        if (x + m_normalTileSize.x * (m_tileState) < 0) break;    // 画面外

        RectF tile(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_normalTileSize);

        if (index == m_settingIndex - 1) {
            tile.set(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_normalTileSize - (m_tileSize * 0.3) * Min(0.0, m_animateState));
            x += ((m_tileSize.x * 0.3) + m_selectedTileMargin) * Min(0.0, m_animateState);
        }

        if (SettingTiles[index] == U"MUSIC") {
            tile.scaled((m_tileState))(m_tile.get(m_index, m_level)).draw(ColorF(1.0, 1.0 - Abs(m_tileState - 1.0)));
        }
        else {
            tile.scaled((m_tileState))(m_tile.get(SettingTiles[index], getData().setting[SettingTiles[index]], Palette::Slategray)).draw(ColorF(1.0, 1.0 - Abs(m_tileState - 1.0)));
        }

        x -= (m_tileMargin + m_normalTileSize.x) * (m_tileState);
    }
}

void Menu::drawSelectedTile() const {
    RectF selectedTile(Arg::bottomCenter = Vec2{ m_selectedTileX, m_tileBaseY }, m_selectedTileSize);
    selectedTile.drawShadow({ 0.0, 0.0 }, 25, 15.0, ColorF(Palette::Gold, 0.5 + Periodic::Sine0_1(2.0s) * 0.5));
    if (m_settingState != 1 || SettingTiles[m_settingIndex] == U"MUSIC") {
        selectedTile(m_tile.get(m_index, m_level, Max(0.0, m_tileOffsetStopwatch.sF() - 1.0))).draw();
    }
    else {
        selectedTile(m_tile.get(SettingTiles[m_settingIndex], getData().setting[SettingTiles[m_settingIndex]], Palette::Slategray)).draw();
    }
}

void Menu::drawUserPlate() const {

}
