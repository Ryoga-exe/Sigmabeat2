#include "Result.hpp"

Result::Result(const InitData& init)
    : IScene(init)
    , m_delta(0.0)
    , m_backgroundTexture(U"assets/images/polygons/0.png")
    , m_fieldRT(360, 1080)
    , m_playing(false) {

    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

    m_index = getData().selectIndex;
    m_level = getData().selectLevel;
    m_judgeRanks = getData().judges;
    m_scorePoint = getData().score;
    m_maxCombo = getData().maxCombo;

    AudioAsset::LoadAsync(U"Result");
}

Result::~Result() {
    AudioAsset::Release(U"Result");
}

void Result::update() {
    if (!m_playing && AudioAsset::IsReady(U"Result")) {
        AudioAsset(U"Result").setLoop(true); 
        AudioAsset(U"Result").play();
        m_playing = true;
    }
}

void Result::draw() const {
    ColorF bgColors[4] = {};
    for (int8 i = 0; i < 4; i++) {
        bgColors[i] = ColorF(m_scores.get(m_index).backgroundColor[i]);
    }
    RectF{ Scene::Size() }.draw(bgColors);
    RectF{ Scene::Size() }(m_backgroundTexture).draw(ColorF(1.0, 1.0, 1.0, 0.3));

    const auto [w, h] = Scene::Size();

    {
        const ScopedRenderTarget2D target(m_fieldRT.clear(ColorF(0, 0, 0, 0)));
        RectF{ Arg::leftCenter(0.0, 360 - 180 + 10.0), 1080.0, 1.5 }.draw(Color(28, 40, 214));
        RectF{ Arg::leftCenter(0.0, 360 + 180 - 10.0), 1080.0, 1.5 }.draw(Color(28, 40, 214));

        FontAsset(U"Game.score")(U"SCORE").draw(Arg::topLeft(50.0, 720 / 3.5), Color(28, 40, 214));
        FontAsset(U"Game.score")(U"SCORE").region(Arg::topLeft(50.0, 720 / 3.5)).bottom().draw(2.0, Color(28, 40, 214));

        FontAsset(U"Game.combo")(U"{}"_fmt(1000000)).draw(Arg::topLeft(50.0, 720 / 3.5 + 40), Color(28, 33, 105));

        FontAsset(U"Game.score")(U"RANK").draw(Arg::topLeft(300.0, 720 / 3.5), Color(28, 40, 214));
        FontAsset(U"Game.score")(U"RANK").region(Arg::topLeft(300.0, 720 / 3.5)).bottom().draw(2.0, Color(28, 40, 214));
    }

    Graphics2D::Flush();
    m_fieldRT.resolve();

    RectF{ Arg::leftCenter(0.0, h / 2.0), static_cast<double>(w), h * 0.5 }.draw(ColorF(1.0, 1.0, 1.0, 0.6));
    RectF{ Arg::leftCenter(0.0, h / 2.0), static_cast<double>(w), h * 0.5 }(m_fieldRT).draw();

    {
        const Color levelColor = Score::LevelColor[m_level].gamma(0.5);
        RectF{ 0, 0, 400, 140 }.drawShadow({0, 2}, 13, 2);
        RectF{ 0, 0, 400, 20 }.draw(Palette::Black);
        RectF{ 0, 20, 400, 60 }.draw(levelColor);
        RectF{ 0, 80, 400, 60 }.draw(Palette::Whitesmoke);
        RectF{ 10, 30, 100, 100 }.stretched(2).drawShadow({ 0, 2 }, 13, 2).draw(Palette::White);
        RectF{ 10, 30, 100, 100 }(m_scores.getTexture(m_index)).draw();

        RectF{ 120, 115, 270, 1.5 }.draw(levelColor);

        RectF{ 0, 0, 400, 140 }.drawFrame(0.0, 1.0, Palette::Whitesmoke, Palette::Whitesmoke);

        RectF{ 330, 25, 65, 65 }.drawShadow({ 0, 2 }, 13, 2).draw(ColorF(0, 0, 0, 0.3)).draw(Palette::Whitesmoke);
        RectF{ 333, 45, 59, 42 }.draw(Color(15, 51, 66));

        FontAsset(U"Tile.detail.small")(U"SPEED : x{:.2f}"_fmt(getData().setting[U"SPEED"].value / (double)getData().setting[U"SPEED"].scale)).draw(Arg::bottomLeft(10, 18), Palette::Lightgray);
        // FontAsset(U"Tile.detail.small")(U"SPEED : x{:.2f}"_fmt(, Palette::Lightgray);


        FontAsset(U"Tile.detail")(U"LEVEL").draw(Arg::bottomCenter(363.0, 45), Color(15, 51, 66));
        FontAsset(U"Tile.title")(m_scores.get(m_index).level[m_level]).draw(Arg::center(362.5, 66), Palette::White);

        FontAsset(U"Tile.detail")(U"TRACK ∞").draw(Vec2{ 120, 25 }, Palette::White);
        
        FontAsset(U"Game.level")(Score::LevelName[m_level].uppercased()).draw(Vec2{120, 35}, Palette::White);

        FontAsset(U"Tile.detail")(m_scores.getTitle(m_index)).draw(Arg::bottomLeft(123, 110), Palette::Dimgray);
        FontAsset(U"Tile.detail.small")(m_scores.getArtist(m_index)).draw(Arg::topLeft(123, 120), Palette::Dimgray);

    }
    
    /*
    {
        const auto [w, h] = Scene::Size();
        const ColorF levelColor(0, 0, 0, 0.5);
        RectF{ Arg::topRight(w, 0), 450, 100 }.drawShadow({ 0, 2 }, 13, 2);
        RectF{ Arg::topRight(w, 0), 450, 20 }.draw(Palette::Black);
        RectF{ Arg::topRight(w, 20), 450, 80 }.draw(levelColor);

        RectF{ Arg::topRight(w, 0), 450, 100 }.drawFrame(0.0, 1.0, Palette::Whitesmoke, Palette::Whitesmoke);
        
        FontAsset(U"Tile.detail.small")(U"MISS : {:>4}"_fmt(m_judgeRanks[4])).draw(Arg::bottomCenter(w - 45, 18), Palette::Gray);
        RectF(w - 90, 3, 0.5, 14).draw(Palette::Lightgray);
        FontAsset(U"Tile.detail.small")(U"LATE : {:>4}"_fmt(m_judgeRanks[3])).draw(Arg::bottomCenter(w - 135, 18), Color(219, 81, 81));
        RectF(w - 180, 3, 0.5, 14).draw(Palette::Lightgray);
        FontAsset(U"Tile.detail.small")(U"FAST : {:>4}"_fmt(m_judgeRanks[2])).draw(Arg::bottomCenter(w - 220, 18), Color(72, 84, 199));
        RectF(w - 260, 3, 0.5, 14).draw(Palette::Lightgray);
        FontAsset(U"Tile.detail.small")(U"GREAT : {:>4}"_fmt(m_judgeRanks[1])).draw(Arg::bottomCenter(w - 305, 18), Palette::White);
        RectF(w - 350, 3, 0.5, 14).draw(Palette::Lightgray);
        FontAsset(U"Tile.detail.small")(U"PERFECT : {:>4}"_fmt(m_judgeRanks[0])).draw(Arg::bottomCenter(w - 400, 18), Color(184, 245, 227));

        FontAsset(U"Tile.detail")(U"SCORE :").draw(Vec2{ w - 440, 60 }, Palette::White);
        FontAsset(U"Game.score")(U"{:>7}"_fmt(m_scorePoint)).draw(Vec2{ w - 380, 35 }, Palette::White);
        FontAsset(U"Tile.detail")(U"MAX COMBO : {:>4}"_fmt(m_maxCombo)).draw(Vec2{ w - 180, 60 }, Palette::White);

    }
    */
}
