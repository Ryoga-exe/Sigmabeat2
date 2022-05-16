﻿#include "Game.hpp"
#include "Utils/Parser.hpp"

Game::Game(const InitData& init)
    : IScene(init)
    , m_hasStarted(false)
    , m_fieldRT(FieldSize)
    , VS(HLSL{ U"example/shader/hlsl/homography.hlsl", U"VS" } | GLSL{ U"example/shader/glsl/homography.vert", {{ U"VSConstants2D", 0 }, { U"VSHomography", 1} } })
    , PS(HLSL{ U"example/shader/hlsl/homography.hlsl", U"PS" } | GLSL{ U"example/shader/glsl/homography.frag", {{ U"PSConstants2D", 0 }, { U"PSHomography", 1} } }) {

    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

    m_speed = 1.00;
    m_combo = 0;
    m_judgementYPos = DefaultJudmentYPos;
    m_laneWidth = (FieldSize.x - 2 * EdgeWidth) / LaneNum;

    if (not (VS && PS)) {
        throw Error{ U"Failed to load shader files" };
    }

    m_speedMap.push_back(SpeedNote{ .timing = 0, .speed = 1.00 });
    m_speedMapIndex = 0;

    loadNotes();

}

void Game::update() {

    if (!m_hasStarted) {
        m_stopwatch.start();
        m_hasStarted = true;
    }

    if (m_hasStarted && !m_speedMap.empty()) {
        if ((m_speedMapIndex + 1 < m_speedMap.size()) && m_speedMap[m_speedMapIndex + 1].timing < m_stopwatch.ms()) {
            m_speedMapIndex++;
        }
    }

    ClearPrint();
    Print << m_stopwatch.elapsed();

    for (auto e : m_speedMap) {
        Print << e.speed;
        //Print << calculateNoteY(e.timing, 1.0);
    }
}

void Game::draw() const {
    drawField();
}

void Game::drawField() const {

    auto [SceneW, SceneH] = Scene::Size();

    auto TopW = SceneW * 0.20;
    auto BottomW = SceneW * 0.70;

    Quad quad{ Vec2{(SceneW - BottomW) / 2, SceneH}, Vec2{(SceneW + BottomW) / 2, SceneH}, Vec2{(SceneW + TopW) / 2, 0}, Vec2{(SceneW - TopW) / 2, 0} };

    {
        const ScopedRenderTarget2D target(m_fieldRT.clear(UI::BackgroundColor));


        for (uint8 i = 1; i < LaneNum; i++) {
            RectF{ Arg::topCenter(EdgeWidth + m_laneWidth * i, 0.0), 2.0, (double)FieldSize.y }.draw(UI::LaneLineColor);
        }


        drawJudmentLine();

        // debug
        // drawNote(Note{ .lane = 0 });

        for (auto e : m_notesMap) {
            drawNote(e);
            // RectF{ Arg::leftCenter(EdgeWidth, calculateNoteY(e.timing, 1.0)), m_laneWidth, 15 }.draw(Palette::White);
        }


        Rect{ Arg::topRight(FieldSize.x, 0), EdgeWidth, FieldSize.y }.draw(UI::EdgeColor);
        Rect{ Arg::topLeft(0, 0), EdgeWidth, FieldSize.y }.draw(UI::EdgeColor);

        Rect{ FieldSize }.draw(Arg::top = ColorF(UI::BlurColor, 0), Arg::bottom = ColorF(UI::BlurColor, 0.8));

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
    RectF{ FieldSize * 0.65 }(m_fieldRT.flipped()).draw();


}

void Game::drawJudmentLine() const {
    
    RectF{ Arg::leftCenter(0, m_judgementYPos), static_cast<double>(FieldSize.x), 7 }.drawShadow({ 0.0, 0.0 }, 12.0, 0.0, ColorF(UI::JudmentLineBlurColor));
    
    RectF{ 0, m_judgementYPos, static_cast<double>(FieldSize.x), -3 }.draw(Arg::top(UI::JudmentLineColor), Arg::bottom(ColorF(UI::JudmentLineBlurColor, 0.5)));
    RectF{ 0, m_judgementYPos, static_cast<double>(FieldSize.x), 3 }.draw(Arg::top(UI::JudmentLineColor), Arg::bottom(ColorF(UI::JudmentLineBlurColor, 0.5)));
    RectF{ Arg::leftCenter(0, m_judgementYPos), static_cast<double>(FieldSize.x), 3 }.draw(ColorF(UI::JudmentLineColor, 0.6));
    RectF{ Arg::leftCenter(0, m_judgementYPos), static_cast<double>(FieldSize.x), 4 }.stretched(2).drawFrame(0.5).stretched(1).drawFrame(0.5, UI::JudmentLineColor);

}

void Game::drawNote(Note note) const {

    

    /*
    RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth, 1100 - cursorY), m_laneWidth, 15 }.draw(Palette::White);
    RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth, 1300 - cursorY), m_laneWidth, 15 }.draw(Palette::White);

    RectF{ Arg::center(EdgeWidth + m_laneWidth + m_laneWidth / 2.0, (1100 + 1300) / 2 - cursorY), m_laneWidth * 0.90, (1300 - 1100) }.draw(ColorF(Palette::White, 0.75));
    */

    if (note.type == 0) {
        double posY = calculateNoteY(note.timing, 1.00);
        RectF{ Arg::leftCenter(EdgeWidth + m_laneWidth * note.lane, posY), m_laneWidth, 15 }.draw(Palette::White);
    }
}

bool Game::loadNotes() {
    auto index = getData().selectIndex;
    auto s_level = getData().selectLevel;
    auto noteStartSeek = m_scores.get(index).noteStartSeek;
    auto bpm = m_scores.getBPM(index);
    int32 level = 0;
    for (uint8 i = 0; i < s_level; i++) {
        if (m_scores.get(index).level[i] > 0) {
            level++;
        }
    }
    m_notesMap.clear();

    TextReader reader(m_scores.get(index).path);
    if (!reader) {
        return false;
    }

    // コメントを除去してから移動
    String content = Utils::RemoveComments(reader.readAll().expandTabs()).substr(noteStartSeek);

    // 指定されたレベルへ移動
    {
        int8 counter = -1;
        size_t pos = 0;
        for (size_t i = 0; i < content.size(); i++) {
            if (content[i] == U'{') {
                counter++;
                pos = i;
                if (counter == level) break;
            }
        }
        if (counter != level) {
            return false;
        }
        content = content.substr(pos).replaced(U'|', U'\n');
    }

    double barLength = 60000 * 4 / bpm;
    double passedTime = m_scores.get(index).offsetMs; // + DELAY;
    m_barMap.push_back(static_cast<int32>(passedTime));
    for (size_t i = 0; i < content.size(); i++) {
        if (content[i] == U'}') {
            break;
        }

        if (content[i] == U' ') {
            continue;
        }

        if (content[i] == U'#') {

        }
        else {
            size_t endPos = content.indexOf(U'\n', i);
            size_t endPosOfLevel = content.indexOf(U'}', i);
            String buffer;

            // because String::npos is too large.
            endPos = Min({ endPos, endPosOfLevel, content.size() });


            buffer = content.substr(i, endPos - i);
            int32 splitCounter = static_cast<int32>(buffer.count(',')) + 1;
            
            Print << buffer;

            int32 counter = 0;
            bool isMacro = false;
            String macro;
            for (auto c : buffer) {
                if (c == '<') {
                    isMacro = true;
                    continue;
                }
                else if (c == '>') {
                    if (macro[0] == 's') {
                        String value = macro.substr(1);
                        Print << value;
                        double speed = ParseOr<double>(value, 1.00);
                        if (speed == 0) {
                            speed = 0.000001;
                        }
                        SpeedNote note;
                        note.speed = speed;
                        note.timing = static_cast<int32>(passedTime + barLength * counter / splitCounter);
                        m_speedMap.push_back(note);
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
                    if (!((c >= '0' && c <= '5') || (c >= 'a' && c <= 'l'))) {
                        continue;
                    }
                    Note note;
                    note.timing = static_cast<int32>(passedTime + barLength * counter / splitCounter);
                    if (c >= '0' && c <= '5') {
                        note.type = 0;
                        note.lane = (uint8)(c - '0');
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
        m_barMap.push_back(static_cast<int32>(passedTime));
    }

    m_endTime = static_cast<int32>(passedTime);
    return true;
}

double Game::calculateNoteY(int32 timing, double speed) const {
    double noteY = m_judgementYPos + ((timing - m_stopwatch.ms()) * m_speedMap[m_speedMapIndex].speed * speed);
    double speedChangeY = 0;
    double prevSpeedY = m_judgementYPos;
    double prevTiming = m_stopwatch.ms();
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
        prevTiming = m_stopwatch.ms();
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
