#include "Menu.hpp"

Menu::Menu(const InitData& init)
    : IScene(init), m_tileOffsetXVelocity(0.0), m_animateState(0.0), m_tileOffsetStopwatch(true){

    m_selectedTileSize = UI::Menu::SelectedTileSize;
    m_tileSize = UI::Menu::NormalIndexSize;
    m_tileMargin = UI::Menu::MarginSize;
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
        constexpr int32 length = 18000;
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

    updateTiles();

}

void Menu::draw() const {

    if (m_indexSize == 0) return;

    //UI::Menu::Tile tile;
    drawTiles();


    // m_test_tile.get(m_selectedIndex, Score::LevelColor[3], Max(0.0, m_tileOffsetStopwatch.sF() - 1.3 )).draw();
    

    // FontAsset(U"Menu")(U"MUSIC SELECT").draw(Arg::topCenter = Point{Scene::Center().x , 0}, Palette::Black);

}

void Menu::updateTiles() {
    auto [centerX, centerY] = Scene::CenterF();
    m_selectedTileSize = UI::Menu::SelectedTileSize - UI::Menu::SizeBetween * Abs(m_animateState);
    m_tileBaseY = centerY + m_tileSize / 3;
    m_selectedTileX = centerX - (UI::Menu::SelectedTileSize / 2 + UI::Menu::NormalIndexSize / 2 + m_tileMargin + UI::Menu::SelectedTileMarginSize) * m_animateState;
}

void Menu::drawTiles() const {


    /*/////////

    ToDo: Tileをレンダーテクスチャ(RenderTexture)を使って書く

    https://siv3d.github.io/ja-jp/news/v041/#1

    /////////*/

    int32 lv = 3;



    // drawSelectedIndex

    RectF selectedTile(Arg::bottomCenter = Vec2{ m_selectedTileX, m_tileBaseY + 200 }, UI::Menu::TileSize);
    selectedTile.drawShadow({ 0.0, 0.0 }, 20, 10.0, Palette::Gold);
    selectedTile(m_test_tile.get(m_selectedIndex, Score::LevelColor[lv], Max(0.0, m_tileOffsetStopwatch.sF() - 1.3))).draw();

    double x = m_selectedTileX + m_selectedTileSize / 2 + m_tileMargin + UI::Menu::SelectedTileMarginSize * (m_animateState >= 0.0 ? 1.0 : 1.0 + m_animateState);

    for (int32 index = m_selectedIndex + 1; index < m_indexSize; index++) {
        
        if (Scene::Width() < x) break;    // 画面外

        RectF tile(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_tileSize);

        if (index == m_selectedIndex + 1) {
            tile.set(Arg::bottomLeft = Vec2{ x, m_tileBaseY }, m_tileSize + UI::Menu::SizeBetween * Max(0.0, m_animateState));
            x += (UI::Menu::SizeBetween + UI::Menu::SelectedTileMarginSize) * Max(0.0, m_animateState);
        }

        tile.stretched(50, 100).movedBy(0, 60).draw(Arg::top = Score::LevelColor[lv], Arg::bottom = ColorF(Score::LevelColor[lv]).gamma(0.5)).drawFrame(3.0);
        tile.stretched(7).drawShadow({ 0.0, 0.0 }, 10, 3.0, Palette::Whitesmoke);
        tile(m_scores.getTexture(index)).draw();

        x += m_tileMargin + m_tileSize;
    }

    x = m_selectedTileX - m_selectedTileSize / 2 - m_tileMargin - UI::Menu::SelectedTileMarginSize * (m_animateState <= 0.0 ? 1.0 : 1.0 - m_animateState);;

    for (int32 index = m_selectedIndex - 1; index >= 0; index--) {
        
        if (x < 0) break;    // 画面外

        RectF tile(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_tileSize);

        if (index == m_selectedIndex - 1) {
            tile.set(Arg::bottomRight = Vec2{ x, m_tileBaseY }, m_tileSize - UI::Menu::SizeBetween * Min(0.0, m_animateState));
            x += (UI::Menu::SizeBetween + UI::Menu::SelectedTileMarginSize) * Min(0.0, m_animateState);
        }

        tile.stretched(50, 100).movedBy(0, 60).draw(Arg::top = Score::LevelColor[lv], Arg::bottom = ColorF(Score::LevelColor[lv]).gamma(0.5)).drawFrame(3.0);
        tile.stretched(10).draw(Palette::Dimgray);
        tile.stretched(7).drawShadow({ 0.0, 0.0 }, 10, 3.0, Palette::Whitesmoke);
        tile(m_scores.getTexture(index)).draw();

        x -= m_tileMargin + m_tileSize;
    }
}