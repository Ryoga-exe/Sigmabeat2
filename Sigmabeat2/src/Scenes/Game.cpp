#include "Game.hpp"
#include "Utils/Parser.hpp"
#include "Config/Config.hpp"

Game::Game(const InitData& init)
    : IScene(init)
    , m_hasStarted(false)
    , m_hasPlayed(false)
    , m_fieldRT(FieldSize)
    , m_tapNoteTexture(U"assets/images/notes/tap.png")
    , m_holdNoteTexture(U"assets/images/notes/hold.png")
    , m_pressNoteTexture(U"assets/images/notes/press.png")
    , m_backgroundTexture(U"assets/images/polygons/0.png")
    , m_pressEffectOpacity(LaneNum, 0.00)
    , m_judgeRanks(5, 0)
    , VS(HLSL{ U"assets/shaders/hlsl/homography.hlsl", U"VS" } | GLSL{ U"assets/shaders/glsl/homography.vert", {{ U"VSConstants2D", 0 }, { U"VSHomography", 1} } })
    , PS(HLSL{ U"assets/shaders/hlsl/homography.hlsl", U"PS" } | GLSL{ U"assets/shaders/glsl/homography.frag", {{ U"PSConstants2D", 0 }, { U"PSHomography", 1} } }) {

    Scene::SetBackground(Palette::Whitesmoke);
    if (!Config::Get().keepSceneSize) {
        Scene::SetLetterbox(Palette::Whitesmoke);
    }

    m_speed = 1.00;
    m_combo = 0;
    m_maxCombo = 0;
    m_scorePoint = 0;
    m_judgementYPos = DefaultJudmentYPos;
    m_laneWidth = (FieldSize.x - 2 * EdgeWidth) / LaneNum;
    m_posMilliSec = 0.00;
    m_stopWatchElapsedMS = -4000;

    if (not (VS && PS)) {
        throw Error{ U"Failed to load shader files" };
    }

    m_speedMap.push_back(SpeedNote{ .timing = 0, .speed = 1.00 });
    m_judgeYMap.push_back(JudgeYNote{ .timing = 0, .posY = DefaultJudmentYPos, .durationMS = 0 });
    m_speedMapIndex = 0;
    m_index = getData().selectIndex;
    m_level = getData().selectLevel;

    getData().judges.fill(0);
    getData().score = 0;


    AudioAsset::Register(U"GameMusic", m_scores.getMusicPath(m_index));
    AudioAsset::LoadAsync(U"GameMusic");

    loadNotes();

    m_stopwatch.reset();

}

Game::~Game() {
    AudioAsset::Unregister(U"GameMusic");
}

void Game::update() {

    m_posMilliSec = AudioAsset(U"GameMusic").posSec() * 1000.0;
    m_stopWatchElapsedMS = m_stopwatch.ms() - 4000;

    m_maxCombo = Max(m_maxCombo, m_combo);

    if (!m_notesMap.empty()) {
        double score_tmp =
            1000000 * ((m_judgeRanks[0] + m_judgeRanks[1] * 0.6 + m_judgeRanks[2] * 0.3 + m_judgeRanks[3] * 0.3) / m_notesMap.size());
        m_scorePoint = static_cast<int32>(score_tmp);
    }

    if (!m_hasStarted && AudioAsset::IsReady(U"GameMusic")) {
        m_stopwatch.start();

        m_hasStarted = true;
    }

    if (m_hasStarted && !m_hasPlayed && m_stopWatchElapsedMS >= 0) {
        AudioAsset(U"GameMusic").play();
        m_hasPlayed = true;
    }

    if (m_endTime + 2000 < m_stopWatchElapsedMS) {
        getData().judges = m_judgeRanks;
        getData().score = m_scorePoint;
        getData().maxCombo = m_maxCombo;
        AudioAsset(U"GameMusic").stop(0.5s);
        changeScene(SceneState::Result, 1.0s);
    }

    // Speed
    if (m_hasStarted && !m_speedMap.empty()) {
        if ((m_speedMapIndex + 1 < m_speedMap.size()) && m_speedMap[m_speedMapIndex + 1].timing < m_stopWatchElapsedMS) {
            m_speedMapIndex++;
        }
    }

    // Judge Bar
    if (m_hasStarted && !m_judgeYMap.empty()) {
        for (size_t i = 1; i < m_judgeYMap.size(); i++) {
            if (m_judgeYMap[i].timing < m_stopWatchElapsedMS) {
                const double t = (double)(m_stopWatchElapsedMS - m_judgeYMap[i].timing) / m_judgeYMap[i].durationMS;
                const double e = EaseOutExpo(t);
                m_judgementYPos = m_judgeYMap[i - 1].posY + (m_judgeYMap[i].posY - m_judgeYMap[i - 1].posY) * e;
            }
        }
    }

    for (uint8 i = 0; i < LaneNum; i++) {
        if (m_keys[i].pressed()) {
            m_pressEffectOpacity[i] = 0.75;
        }
        else {
            m_pressEffectOpacity[i] -= 0.05;
            m_pressEffectOpacity[i] = Max(m_pressEffectOpacity[i], 0.00);
        }
    }

    if (KeyEscape.pressed()) {
        AudioAsset(U"GameMusic").stop(0.7s);
        changeScene(SceneState::Menu, 1.0s);
    }

    if (getData().isAuto) {
        judgementAuto();
    }
    else {
        judgement();
    }
    
}

void Game::draw() const {

    ColorF bgColors[4] = {};
    for (int8 i = 0; i < 4; i++) {
        bgColors[i] = ColorF(m_scores.get(m_index).backgroundColor[i]);
    }
    RectF{ Scene::Size() }.draw(bgColors);
    RectF{ Scene::Size() }(m_backgroundTexture).draw(ColorF(1.0, 1.0, 1.0, 0.3));
    drawField();

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
}

void Game::drawField() const {

    auto [SceneW, SceneH] = Scene::Size();

    auto TopW = SceneW * 0.20;
    auto BottomW = SceneW * 0.70;

    Quad quad{ Vec2{(SceneW - TopW) / 2, 0}, Vec2{(SceneW + TopW) / 2, 0}, Vec2{(SceneW + BottomW) / 2, SceneH}, Vec2{(SceneW - BottomW) / 2, SceneH} };

    {
        const ScopedRenderTarget2D target(m_fieldRT.clear(UI::BackgroundColor));


        for (uint8 i = 1; i < LaneNum; i++) {
            RectF{ Arg::topCenter(EdgeWidth + m_laneWidth * i, 0.0), 2.0, (double)FieldSize.y }.draw(UI::LaneLineColor);
        }

        FontAsset(U"Menu")(U"COMBO").drawAt(FieldSize.x / 2.0, convertPosY(380), Palette::White);
        FontAsset(U"Game.combo")(m_combo).drawAt(FieldSize.x / 2.0, convertPosY(300), Palette::White);

        drawPressEffect();

        drawJudmentLine();

        drawBars();

        drawNotes();

        m_effect.update();

        Rect{ Arg::topLeft(0, 0), EdgeWidth, FieldSize.y }.draw(UI::EdgeColor);
        Rect{ Arg::topRight(FieldSize.x, 0), EdgeWidth+2, FieldSize.y }.draw(UI::EdgeColor);

        Rect{ FieldSize }.draw(Arg::top = ColorF(UI::BlurColor, 0.8), Arg::bottom = ColorF(UI::BlurColor, 0));

    }

    /*--------------------------*/

    Graphics2D::Flush();
    m_fieldRT.resolve();

    ConstantBuffer<Homography> vsHomography;
    ConstantBuffer<Homography> psHomography;

    {
        const ScopedCustomShader2D shader{ VS, PS };
        const ScopedRenderStates2D sampler{ SamplerState::ClampAniso };
        {
            const Mat3x3 mat = Mat3x3::Homography(quad);
            vsHomography = { Float4{ mat._11_12_13, 0 }, Float4{ mat._21_22_23, 0 }, Float4{ mat._31_32_33, 0 } };
            Graphics2D::SetVSConstantBuffer(1, vsHomography);

            const Mat3x3 inv = mat.inverse();
            psHomography = { Float4{ inv._11_12_13, 0 }, Float4{ inv._21_22_23, 0 }, Float4{ inv._31_32_33, 0 } };
            Graphics2D::SetPSConstantBuffer(1, psHomography);

            Rect{ 1 }(m_fieldRT).draw();
        }
    }


    // debug
    // RectF{ FieldSize * 0.65 }(m_fieldRT).draw();


}

void Game::drawPressEffect() const {
    for (uint8 i = 0; i < LaneNum; i++) {
        RectF{ Arg::bottomLeft(EdgeWidth + m_laneWidth * i, convertPosY(0)), m_laneWidth, 180 }
            .draw(Arg::top = ColorF(Palette::Aliceblue, 0.00), Arg::bottom = ColorF(Palette::Aliceblue, m_pressEffectOpacity[i]));
    }
}

void Game::drawJudmentLine() const {
    
    RectF{ Arg::leftCenter(0, convertPosY(m_judgementYPos)), static_cast<double>(FieldSize.x), 7 }.drawShadow({ 0.0, 0.0 }, 12.0, 0.0, ColorF(UI::JudmentLineBlurColor));
    
    RectF{ 0, convertPosY(m_judgementYPos), static_cast<double>(FieldSize.x), -3 }.draw(Arg::top(UI::JudmentLineColor), Arg::bottom(ColorF(UI::JudmentLineBlurColor, 0.5)));
    RectF{ 0, convertPosY(m_judgementYPos), static_cast<double>(FieldSize.x), 3 }.draw(Arg::top(UI::JudmentLineColor), Arg::bottom(ColorF(UI::JudmentLineBlurColor, 0.5)));
    RectF{ Arg::leftCenter(0, convertPosY(m_judgementYPos)), static_cast<double>(FieldSize.x), 3 }.draw(ColorF(UI::JudmentLineColor, 0.6));
    RectF{ Arg::leftCenter(0, convertPosY(m_judgementYPos)), static_cast<double>(FieldSize.x), 4 }.stretched(2).drawFrame(0.5).stretched(1).drawFrame(0.5, UI::JudmentLineColor);

}

void Game::drawBars() const {
    for (const auto& e : m_barMap) {
        double posY = calculateNoteY(e, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale);
        RectF{ Arg::leftCenter(EdgeWidth, convertPosY(posY)), m_laneWidth * LaneNum, 2.0 }.draw(UI::LaneLineColor);
        if (posY > DrawLimitY) {
            break;
        }
    }
}

void Game::drawNotes() const {
    for (const auto& e : m_notesMap) {
        if (!drawNote(e)) {
            break;
        }
    }
}

bool Game::drawNote(Note note) const {
    double posY = calculateNoteY(note.timing, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale);
    if (note.type == NoteType::Tap || note.type == NoteType::PassedTap) {
        RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth * note.lane, convertPosY(posY)), m_laneWidth, 15 }(m_tapNoteTexture).draw();
    }
    if (note.type == NoteType::HoldStart || note.type == NoteType::PressedHoldStart) {
        double startPosY = posY;
        double startPosEndY = Min(calculateNoteY(note.sub, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale), (double)DrawLimitY);

        RectF{ Arg::bottomCenter(EdgeWidth + m_laneWidth * note.lane + m_laneWidth / 2.0, convertPosY((startPosEndY + startPosY) / 2)), m_laneWidth * 0.90, (startPosEndY - startPosY) / 2.0 }
            .draw(Arg::top = Color(116, 111, 217, 190), Arg::bottom = Color(19, 189, 225, 190));
        RectF{ Arg::topCenter(EdgeWidth + m_laneWidth * note.lane + m_laneWidth / 2.0, convertPosY((startPosEndY + startPosY) / 2)), m_laneWidth * 0.90, (startPosEndY - startPosY) / 2.0 }
            .draw(Arg::top = Color(19, 189, 225, 190), Arg::bottom = Color(242, 124, 208, 190));

        RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth * note.lane, convertPosY(startPosY)), m_laneWidth, 15 }(m_holdNoteTexture).draw();
        RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth * note.lane, convertPosY(startPosEndY)), m_laneWidth, 15 }(m_holdNoteTexture).draw();
    }
    if (note.type == NoteType::PassedHoldStart) {
        double startPosY = posY;
        double startPosEndY = Min(calculateNoteY(note.sub, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale), (double)DrawLimitY);

        RectF{ Arg::bottomCenter(EdgeWidth + m_laneWidth * note.lane + m_laneWidth / 2.0, convertPosY((startPosEndY + startPosY) / 2)), m_laneWidth * 0.90, (startPosEndY - startPosY) / 2.0 }
        .draw(Arg::top = Color(71, 68, 130, 190), Arg::bottom = Color(12, 109, 130, 190));
        RectF{ Arg::topCenter(EdgeWidth + m_laneWidth * note.lane + m_laneWidth / 2.0, convertPosY((startPosEndY + startPosY) / 2)), m_laneWidth * 0.90, (startPosEndY - startPosY) / 2.0 }
        .draw(Arg::top = Color(12, 109, 130, 190), Arg::bottom = Color(138, 72, 119, 190));

        RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth * note.lane, convertPosY(startPosY)), m_laneWidth, 15 }(m_holdNoteTexture).draw();
        RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth * note.lane, convertPosY(startPosEndY)), m_laneWidth, 15 }(m_holdNoteTexture).draw();
    }
    if (note.type == NoteType::Press || note.type == NoteType::PassedPress) {
        RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth * note.lane, convertPosY(posY)), m_laneWidth, 15 }(m_pressNoteTexture).draw();
    }

    return posY <= DrawLimitY;
}

bool Game::loadNotes() {
    auto s_level = getData().selectLevel;
    auto noteStartSeek = m_scores.get(m_index).noteStartSeek;
    auto bpm = m_scores.getBPM(m_index);
    int32 level = 0;
    for (uint8 i = 0; i < s_level; i++) {
        if (m_scores.get(m_index).level[i] > 0) {
            level++;
        }
    }
    m_notesMap.clear();

    TextReader reader(m_scores.get(m_index).path);
    if (!reader) {
        return false;
    }

    // 移動してからコメントを除去
    String content = Utils::RemoveComments(reader.readAll().expandTabs().substr(noteStartSeek));

    // 指定されたレベルへ移動
    {
        int8 counter = -1;
        size_t pos = 0;
        for (size_t i = 0; i < content.size(); i++) {
            if (content[i] == U'{') {
                counter++;
                pos = i + 1;
                if (counter == level) break;
            }
        }
        if (counter != level) {
            return false;
        }
        content = content.substr(pos).replaced(U'|', U'\n');
    }

    double barLength = 60000 * 4 / bpm;
    double passedTime = m_scores.get(m_index).offsetMs; // + DELAY;
    double funcDelayMS = 0.0;
    bool enableBar = true;
    m_barMap.push_back(static_cast<int32>(passedTime));
    for (size_t i = 0; i < content.size(); i++) {
        if (content[i] == U'}') {
            break;
        }

        if (content[i] == U' ') {
            continue;
        }

        if (content[i] == U'#') {
            String tagname;
            size_t pos = content.indexOf(U" ", i);
            size_t newlinePos = content.indexOf(U"\n", i);
            if (pos == String::npos) break;
            if (pos > newlinePos) {
                i = newlinePos;
                continue;
            }
            tagname = content.substr(i, pos - i);
            tagname.uppercase();

            String tagvalue = content.substr(pos, newlinePos - pos);
            tagvalue.trim();

            if (tagname == U"#FUNCDELAY") {
                auto values = tagvalue.split(U'/');
                if (values.size() >= 2) {
                    int32 p = ParseOr<int32>(values[0], 0);
                    int32 q = ParseOr<int32>(values[1], 1);
                    if (q == 0) {
                        funcDelayMS = 0.00;
                    }
                    else {
                        funcDelayMS = (barLength * (double)p / (double)q);
                    }
                }
                else {
                    funcDelayMS = 0.00;
                }
            }
            if (tagname == U"#FUNCDELAYMS") {
                funcDelayMS = ParseOr<double>(tagvalue, 0.00);
            }
            if (tagname == U"#BPM" || tagname == U"#BPMCHANGE") {
                auto values = (tagvalue).split(U' ');
                bpm = ParseOr<double>(values[0], bpm);
                barLength = 60000 * 4 / bpm;
            }
            if (tagname == U"#SPEED") {
                applyMacro('s', tagvalue, static_cast<int32>(passedTime + funcDelayMS));
            }
            if (tagname == U"#JUDGEBAR") {
                applyMacro('j', tagvalue, static_cast<int32>(passedTime + funcDelayMS));
            }
            i = newlinePos;
            continue;
        }
        else {
            size_t endPos = content.indexOf(U'\n', i);
            size_t endPosOfLevel = content.indexOf(U'}', i);
            String buffer;

            // because String::npos is too large.
            endPos = Min({ endPos, endPosOfLevel, content.size() });


            buffer = content.substr(i, endPos - i);
            int32 splitCounter = static_cast<int32>(buffer.count(',')) + 1;
            
            int32 counter = 0;
            bool isMacro = false;
            String macro;
            for (auto c : buffer) {
                if (c == '<') {
                    isMacro = true;
                    continue;
                }
                else if (c == '>') {
                    applyMacro(macro[0], macro.substr(1).trimmed(), static_cast<int32>(passedTime + barLength * counter / splitCounter));

                    if (macro[0] == 'e') {  // switch bar
                        enableBar ^= 1;
                    }
                    if (macro[0] == 'b') {  // bpm
                        auto values = (macro.substr(1)).split(U' ');
                        bpm = ParseOr<double>(values[0], bpm);

                        barLength = 60000 * 4 / bpm;
                    }
                    macro.clear();
                    isMacro = false;
                }

                if (isMacro) {
                    macro << c;
                }
                else if (c == ',') {
                    counter++;
                    continue;
                }
                else {
                    if (!((c >= '0' && c <= '5') || (c >= 'a' && c <= 'r'))) {
                        continue;
                    }
                    Note note;
                    note.timing = static_cast<int32>(passedTime + barLength * counter / splitCounter);
                    if ('0' <= c && c <= '5') {
                        note.type = NoteType::Tap;
                        note.lane = (uint8)(c - '0');
                    }
                    if ('a' <= c && c <= 'f') {
                        note.type = NoteType::HoldStart;
                        note.lane = (uint8)(c - 'a');
                    }
                    if ('g' <= c && c <= 'l') {
                        note.type = NoteType::HoldEnd;
                        note.lane = (uint8)(c - 'g');
                    }
                    if ('m' <= c && c <= 'r') {
                        note.type = NoteType::Press;
                        note.lane = (uint8)(c - 'm');
                    }
                    m_notesMap.push_back(note);
                }
            }

            i = endPos;
            if (endPos == endPosOfLevel) {
                break;
            }
        }
        passedTime += barLength;
        if (enableBar) {
            m_barMap.push_back(static_cast<int32>(passedTime));
        }
    }

    Array<std::stack<int32>> longNotes(LaneNum);
    for (int32 i = static_cast<int32>(m_notesMap.size()) - 1; i >= 0; i--) {
        if (m_notesMap[i].type == NoteType::HoldEnd) {
            longNotes[m_notesMap[i].lane].push(m_notesMap[i].timing);
        }
        if (m_notesMap[i].type == NoteType::HoldStart && !longNotes[m_notesMap[i].lane].empty()) {
            m_notesMap[i].sub = longNotes[m_notesMap[i].lane].top();
            longNotes[m_notesMap[i].lane].pop();
        }
    }

    m_endTime = static_cast<int32>(passedTime);
    return true;
}

bool Game::applyMacro(String::value_type macro, String value, int32 timing, String::value_type spliter) {
    switch (macro) {
    case 's':  // speed
        {
            double speed = ParseOr<double>(value, 1.00);
            if (speed == 0) {
                speed = 0.000001;
            }
            SpeedNote note;
            note.speed = speed;
            note.timing = timing;
            m_speedMap.push_back(note);
        }
        break;
    case 'j':  // judgementYpos
        {
            auto values = (value).split(spliter);
            if (values.size() == 2) {
                double posY = Math::Clamp(ParseOr<double>(values[0], DefaultJudmentYPos), 0.0, 1000.0);
                int32 durationMS = ParseOr<int32>(values[1], 1000);
                JudgeYNote note;
                note.timing = timing;
                note.durationMS = durationMS;
                note.posY = posY;
                m_judgeYMap.push_back(note);
            }
        }
        break;
    case 'l':  // add bar
        {
            m_barMap.push_back(timing);
        }
        break;
    case 'e':  // switch bar
        {
            
        }
        break;
    case 'b':  // bpm
        {
            
        }
        break;
    case 'c':  // background color
        {
            auto values = (value.substr(1)).split(spliter);
        }
        break;
    default:
        return false;
    }
    return true;
}

double Game::calculateNoteY(int32 timing, double speed) const {
    speed *= 0.25;
    double noteY = m_judgementYPos + ((timing - m_stopWatchElapsedMS) * m_speedMap[m_speedMapIndex].speed * speed);
    double speedChangeY = 0;
    double prevSpeedY = m_judgementYPos;
    double prevTiming = m_stopWatchElapsedMS;
    for (size_t i = m_speedMapIndex + 1; i < m_speedMap.size(); i++) {
        speedChangeY = prevSpeedY + ((m_speedMap[i].timing - prevTiming) * m_speedMap[i - 1].speed * speed);

        if (m_speedMap[i].timing < timing) {
            double duration = noteY - speedChangeY;
            noteY = speedChangeY + duration * (m_speedMap[i].speed / m_speedMap[i - 1].speed);
            prevSpeedY = speedChangeY;
            prevTiming = m_speedMap[i].timing;
        }
        else {
            break;
        }
    }
    if (noteY < m_judgementYPos) {
        prevSpeedY = m_judgementYPos;
        prevTiming = m_stopWatchElapsedMS;
        for (size_t i = m_speedMapIndex; i > 0; i--) {
            speedChangeY = prevSpeedY - ((prevTiming - m_speedMap[i].timing) * m_speedMap[i].speed * speed);

            if (m_speedMap[i].timing > timing) {
                double dur = noteY - speedChangeY;
                noteY = speedChangeY + dur * (m_speedMap[i - 1].speed / m_speedMap[i].speed);
                prevSpeedY = speedChangeY;
                prevTiming = m_speedMap[i].timing;
            }
            else break;
        }
    }
    return noteY;
}

void Game::judgement() {
    Array<bool> hasJudged(LaneNum, false);
    for (auto& e : m_notesMap) {
        if (e.type == NoteType::PassedTap) {
            double posY = calculateNoteY(e.timing, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale);
            if (posY < -30.0) {
                e.type = NoteType::None;
            }
        }
        if (e.type == NoteType::PassedHoldStart) {
            double posY = calculateNoteY(e.sub, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale);
            if (posY < -30.0) {
                e.type = NoteType::None;
            }
        }
        Vec2 effectPos{ EdgeWidth + m_laneWidth * e.lane + m_laneWidth / 2.0, convertPosY(DefaultJudmentYPos + 80) };
        if (e.type == NoteType::Tap) {
            int32 noteFar = e.timing - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;
            if (noteFar < -JudgeFarMS[3]) {
                m_effect.add<JudgeEffect>(effectPos, U"MISS", FontAsset(U"Tile.detail"), Palette::Gray);
                m_judgeRanks[4]++;
                m_combo = 0;
                e.type = NoteType::PassedTap;
            }
            if (m_keys[e.lane].down() && !hasJudged[e.lane]) {

                if (Abs(noteFar) <= JudgeFarMS[3]) {
                    e.type = NoteType::None;
                    hasJudged[e.lane] = true;
                    if (Abs(noteFar) <= JudgeFarMS[2]) {
                        m_combo++;
                        if (Abs(noteFar) <= JudgeFarMS[1]) {
                            if (Abs(noteFar) <= JudgeFarMS[0]) {
                                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                                m_judgeRanks[0]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"GREAT", FontAsset(U"Tile.detail"));
                                m_judgeRanks[1]++;
                            }
                        }
                        else {
                            if (noteFar < 0) {
                                m_effect.add<JudgeEffect>(effectPos, U"LATE", FontAsset(U"Tile.detail"), Color(219, 81, 81));
                                m_judgeRanks[3]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"FAST", FontAsset(U"Tile.detail"), Color(72, 84, 199));
                                m_judgeRanks[2]++;
                            }
                        }
                    }
                    else {
                        m_effect.add<JudgeEffect>(effectPos, U"MISS", FontAsset(U"Tile.detail"), Palette::Gray);
                        m_judgeRanks[4]++;
                        m_combo = 0;
                    }
                }
            }
        }
        if (e.type == NoteType::HoldStart) {
            int32 noteFar = e.timing - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;
            if (noteFar < -JudgeFarMS[3]) {
                m_effect.add<JudgeEffect>(effectPos, U"MISS", FontAsset(U"Tile.detail"), Palette::Gray);
                m_judgeRanks[4]++;
                m_combo = 0;
                e.type = NoteType::PassedHoldStart;
            }
            if (m_keys[e.lane].down() && !hasJudged[e.lane]) {
                if (Abs(noteFar) <= JudgeFarMS[3]) {
                    e.type = NoteType::PressedHoldStart;
                    hasJudged[e.lane] = true;
                    if (Abs(noteFar) <= JudgeFarMS[2]) {
                        m_combo++;
                        if (Abs(noteFar) <= JudgeFarMS[1]) {
                            if (Abs(noteFar) <= JudgeFarMS[0]) {
                                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                                m_judgeRanks[0]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"GREAT", FontAsset(U"Tile.detail"));
                                m_judgeRanks[1]++;
                            }
                        }
                        else {
                            if (noteFar < 0) {
                                m_effect.add<JudgeEffect>(effectPos, U"LATE", FontAsset(U"Tile.detail"), Color(219, 81, 81));
                                m_judgeRanks[3]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"FAST", FontAsset(U"Tile.detail"), Color(72, 84, 199));
                                m_judgeRanks[2]++;
                            }
                        }
                    }
                    else {
                        m_effect.add<JudgeEffect>(effectPos, U"MISS", FontAsset(U"Tile.detail"), Palette::Gray);
                        m_judgeRanks[4]++;
                        m_combo = 0;
                        e.type = NoteType::PassedHoldStart;
                    }
                }
            }
        }
        if (e.type == NoteType::PressedHoldStart) {
            e.timing = m_stopWatchElapsedMS;
            int32 noteFar = e.sub - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;

            if (noteFar <= 0) {
                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                m_judgeRanks[0]++;
                e.type = NoteType::None;
                m_combo++;
            }
            if (m_keys[e.lane].up()) {
                if (noteFar <= JudgeFarMS[2]) {
                    m_combo++;
                    e.type = NoteType::None;
                    if (Abs(noteFar) <= JudgeFarMS[1]) {
                        if (Abs(noteFar) <= JudgeFarMS[0]) {
                            m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                            m_judgeRanks[0]++;
                        }
                        else {
                            m_effect.add<JudgeEffect>(effectPos, U"GREAT", FontAsset(U"Tile.detail"));
                            m_judgeRanks[1]++;
                        }
                    }
                    else {
                        m_effect.add<JudgeEffect>(effectPos, U"FAST", FontAsset(U"Tile.detail"), Color(72, 84, 199));
                        m_judgeRanks[2]++;
                    }
                }
                else {
                    m_effect.add<JudgeEffect>(effectPos, U"MISS", FontAsset(U"Tile.detail"), Palette::Gray);
                    m_judgeRanks[4]++;
                    e.type = NoteType::PassedHoldStart;
                    m_combo = 0;
                }
            }
        }
        if (e.type == NoteType::Press) {
            int32 noteFar = e.timing - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;
            if (noteFar < -JudgeFarMS[3]) {
                m_effect.add<JudgeEffect>(effectPos, U"MISS", FontAsset(U"Tile.detail"), Palette::Gray);
                m_judgeRanks[4]++;
                m_combo = 0;
                e.type = NoteType::PassedPress;
            }
            if (Abs(noteFar) <= JudgeFarMS[0]) {
                if (m_keys[e.lane].pressed()) {
                    m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                    m_judgeRanks[0]++;
                    e.type = NoteType::None;
                    m_combo++;
                }
            }
            if (noteFar < -JudgeFarMS[1]) {
                if (m_keys[e.lane].pressed()) {
                    e.type = NoteType::None;
                    if (Abs(noteFar) <= JudgeFarMS[2]) {
                        m_combo++;
                        if (Abs(noteFar) <= JudgeFarMS[1]) {
                            if (Abs(noteFar) <= JudgeFarMS[0]) {
                                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                                m_judgeRanks[0]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"GREAT", FontAsset(U"Tile.detail"));
                                m_judgeRanks[1]++;
                            }
                        }
                        else {
                            if (noteFar < 0) {
                                m_effect.add<JudgeEffect>(effectPos, U"LATE", FontAsset(U"Tile.detail"), Color(219, 81, 81));
                                m_judgeRanks[3]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"FAST", FontAsset(U"Tile.detail"), Color(72, 84, 199));
                                m_judgeRanks[2]++;
                            }
                        }
                    }
                    else {
                        m_effect.add<JudgeEffect>(effectPos, U"MISS", FontAsset(U"Tile.detail"), Palette::Gray);
                        m_judgeRanks[4]++;
                        e.type = NoteType::None;
                        m_combo = 0;
                    }
                }
            }

            if (m_keys[e.lane].up()) {
                if (Abs(noteFar) <= JudgeFarMS[3]) {
                    e.type = NoteType::None;
                    if (Abs(noteFar) <= JudgeFarMS[2]) {
                        m_combo++;
                        if (Abs(noteFar) <= JudgeFarMS[1]) {
                            if (Abs(noteFar) <= JudgeFarMS[0]) {
                                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                                m_judgeRanks[0]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"GREAT", FontAsset(U"Tile.detail"));
                                m_judgeRanks[1]++;
                            }
                        }
                        else {
                            if (noteFar < 0) {
                                m_effect.add<JudgeEffect>(effectPos, U"LATE", FontAsset(U"Tile.detail"), Color(219, 81, 81));
                                m_judgeRanks[3]++;
                            }
                            else {
                                m_effect.add<JudgeEffect>(effectPos, U"FAST", FontAsset(U"Tile.detail"), Color(72, 84, 199));
                                m_judgeRanks[2]++;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Game::judgementAuto() {
    for (auto& e : m_notesMap) {
        if (e.type == NoteType::PassedTap) {
            double posY = calculateNoteY(e.timing, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale);
            if (posY < -30.0) {
                e.type = NoteType::None;
            }
        }
        if (e.type == NoteType::PassedHoldStart) {
            double posY = calculateNoteY(e.sub, getData().setting[U"SPEED"].value / getData().setting[U"SPEED"].scale);
            if (posY < -30.0) {
                e.type = NoteType::None;
            }
        }
        Vec2 effectPos{ EdgeWidth + m_laneWidth * e.lane + m_laneWidth / 2.0, convertPosY(DefaultJudmentYPos + 80) };
        if (e.type == NoteType::Tap) {
            int32 noteFar = e.timing - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;
            if (noteFar < 0) {
                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                m_judgeRanks[0]++;
                m_combo++;
                e.type = NoteType::None;
            }
        }
        if (e.type == NoteType::HoldStart) {
            int32 noteFar = e.timing - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;
            if (noteFar < 0) {
                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                m_judgeRanks[0]++;
                m_combo++;
                e.type = NoteType::PressedHoldStart;
            }
        }
        if (e.type == NoteType::PressedHoldStart) {
            e.timing = m_stopWatchElapsedMS;
            int32 noteFar = e.sub - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;

            if (noteFar <= 0) {
                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                m_judgeRanks[0]++;
                e.type = NoteType::None;
                m_combo++;
            }
        }
        if (e.type == NoteType::Press) {
            int32 noteFar = e.timing - m_stopWatchElapsedMS + getData().setting[U"TIMING"].value;
            if (noteFar < 0) {
                m_effect.add<JudgeEffect>(effectPos, U"PERFECT", FontAsset(U"Tile.detail"), Color(184, 245, 227));
                m_judgeRanks[0]++;
                m_combo++;
                e.type = NoteType::None;
            }
        }
    }
}
