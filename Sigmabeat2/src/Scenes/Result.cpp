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
    m_delta += Scene::DeltaTime() * 100;

    if (!m_playing && AudioAsset::IsReady(U"Result")) {
        AudioAsset(U"Result").setLoop(true); 
        AudioAsset(U"Result").play();
        m_playing = true;
    }

    if (m_delta >= 100 && !Keyboard::GetAllInputs().empty()) {
        AudioAsset(U"Result").stop(0.5s);
        changeScene(SceneState::Menu, 1.0s);
    }
}

void Result::draw() const {

    const double t = m_delta / 75.0;
    const int32 desplayScore = static_cast<int32>(m_scorePoint * Math::Clamp(EaseOutCubic(t), 0.0, 1.0));

    ColorF bgColors[4] = {};
    for (int8 i = 0; i < 4; i++) {
        bgColors[i] = ColorF(m_scores.get(m_index).backgroundColor[i]);
    }
    RectF{ Scene::Size() }.draw(bgColors);
    RectF{ Scene::Size() }(m_backgroundTexture).draw(ColorF(1.0, 1.0, 1.0, 0.3));

    const auto [w, h] = Scene::Size();

    RectF{ Arg::leftCenter(0.0, h / 2.0), static_cast<double>(w), h * 0.5 }.draw(ColorF(1.0, 1.0, 1.0, 0.7));
    RectF{ Arg::leftCenter(0.0, h / 2.0), static_cast<double>(w), h * 0.5 }(m_fieldRT).draw();

    RectF{ Arg::leftCenter(0.0, h / 2.0 - h / 4.0 + 10.0), static_cast<double>(w), 1.5 }.draw(Color(28, 40, 214));
    RectF{ Arg::leftCenter(0.0, h / 2.0 + h / 4.0 - 10.0), static_cast<double>(w), 1.5 }.draw(Color(28, 40, 214));

    FontAsset(U"Game.score")(U"SCORE").draw(Arg::topLeft(50.0, h / 3.5), Color(28, 40, 214));
    FontAsset(U"Game.score")(U"SCORE").region(Arg::topLeft(50.0, h / 3.5)).bottom().draw(2.0, Color(28, 40, 214));

    FontAsset(U"Game.combo")(U"{}"_fmt(desplayScore)).draw(Arg::topLeft(50.0, h / 3.5 + 40), Color(28, 33, 105));

    FontAsset(U"Game.score")(U"MAX COMBO : {}"_fmt(m_maxCombo)).draw(Arg::topLeft(50.0, h / 3.5 + 200), Color(96, 96, 102));

    FontAsset(U"Game.score")(U"RANK").draw(Arg::topLeft(750.0, h / 3.5), Color(28, 40, 214));
    FontAsset(U"Game.score")(U"RANK").region(Arg::topLeft(750.0, h / 3.5)).bottom().draw(2.0, Color(28, 40, 214));

    FontAsset(U"Game.combo")(getRank(desplayScore)).draw(Arg::topLeft(750.0, h / 3.5 + 40), Color(28, 33, 105));

    RectF{ Arg::topRight(480, h / 3.5 + 300 - 18), 450, 20 }.draw(ColorF(0.0, 0.0, 0.0, 0.3));

    FontAsset(U"Tile.detail.small")(U"MISS : {:>4}"_fmt(m_judgeRanks[4])).draw(Arg::bottomCenter(480 - 45, h / 3.5 + 300), Palette::Gray);
    RectF(480 - 90, h / 3.5 + 285, 0.5, 14).draw(Palette::Lightgray);
    FontAsset(U"Tile.detail.small")(U"LATE : {:>4}"_fmt(m_judgeRanks[3])).draw(Arg::bottomCenter(480 - 135, h / 3.5 + 300), Color(219, 81, 81));
    RectF(480 - 180, h / 3.5 + 285, 0.5, 14).draw(Palette::Lightgray);
    FontAsset(U"Tile.detail.small")(U"FAST : {:>4}"_fmt(m_judgeRanks[2])).draw(Arg::bottomCenter(480 - 220, h / 3.5 + 300), Color(72, 84, 199));
    RectF(480 - 260, h / 3.5 + 285, 0.5, 14).draw(Palette::Lightgray);
    FontAsset(U"Tile.detail.small")(U"GREAT : {:>4}"_fmt(m_judgeRanks[1])).draw(Arg::bottomCenter(480 - 305, h / 3.5 + 300), Palette::White);
    RectF(480 - 350, h / 3.5 + 285, 0.5, 14).draw(Palette::Lightgray);
    FontAsset(U"Tile.detail.small")(U"PERFECT : {:>4}"_fmt(m_judgeRanks[0])).draw(Arg::bottomCenter(480 - 400, h / 3.5 + 300), Color(184, 245, 227));


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
}

const String Result::getRank(const int32 scorePoint) const {
    String ret;
    if (scorePoint >= 1000000) ret = U"Σ";
    else if (scorePoint >= 980000) ret = U"Χ";
    else if (scorePoint >= 950000) ret = U"S+";
    else if (scorePoint >= 900000) ret = U"S";
    else if (scorePoint >= 850000) ret = U"A+";
    else if (scorePoint >= 800000) ret = U"A";
    else if (scorePoint >= 750000) ret = U"B+";
    else if (scorePoint >= 700000) ret = U"B";
    else if (scorePoint >= 600000) ret = U"C";
    else if (scorePoint >= 500000) ret = U"D";
    else if (scorePoint >= 400000) ret = U"E";
    else ret = U"F";
    return ret;
}
