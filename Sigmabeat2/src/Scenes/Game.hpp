#pragma once
#include "Common.hpp"
#include "Base/Singleton.hpp"
#include "Score/Manager.hpp"
#include "Score/Note/Note.hpp"

namespace UI {
    constexpr Color BackgroundColor(Palette::Black);
    constexpr Color EdgeColor(Palette::Aliceblue);
    constexpr Color LaneLineColor(Palette::Darkgray);
    constexpr Color BlurColor(Palette::Black);
    constexpr Color JudmentLineColor(252, 255, 252);
    constexpr Color JudmentLineBlurColor(Palette::Aliceblue);

}

class Game : public MyApp::Scene {
public:
    Game(const InitData& init);
    ~Game();
    void update() override;
    void draw() const override;


private:
    inline constexpr int32 convertPosY(int32 y) const {
        return FieldSize.y - y;
    }
    inline constexpr double convertPosY(double y) const {
        return static_cast<double>(FieldSize.y) - y;
    }
    void drawField() const;
    void drawBars() const;
    void drawNotes() const;
    bool drawNote(Note note) const;
    void drawPressEffect() const;
    void drawJudmentLine() const;
    bool loadNotes();
    bool applyMacro(String::value_type macro, String value, int32 timing, String::value_type spliter = U' ');
    double calculateNoteY(int32 timing, double speed) const;

    struct Homography {
        Float4 m1;
        Float4 m2;
        Float4 m3;
    };

    Score::Manager& m_scores = Singleton<Score::Manager>::get_instance();

    const Size FieldSize = { 500, 1000 };
    const int32 DrawLimitY = FieldSize.y * 2;
    const VertexShader VS;
    const PixelShader PS;
    const double DefaultJudmentYPos = 60.00;
    const int32 EdgeWidth = 30;
    const int8 LaneNum = 6;

    MSRenderTexture m_fieldRT;

    Stopwatch m_stopwatch;
    double m_posMilliSec;
    int32 m_stopWatchElapsedMS;

    Array<Note> m_notesMap;
    Array<SpeedNote> m_speedMap;
    Array<JudgeYNote> m_judgeYMap;
    Array<int32> m_barMap;
    Array<Input> m_keys = {
        KeyS,
        KeyD,
        KeyF,
        KeyJ,
        KeyK,
        KeyL,
    };
    Array<double> m_pressEffectOpacity;

    const Texture m_tapNoteTexture;
    const Texture m_holdNoteTexture;
    const Texture m_pressNoteTexture;

    double m_judgementYPos;
    double m_laneWidth;
    double m_speed;
    int32 m_combo;
    int32 m_endTime;
    int32 m_speedMapIndex;
    int32 m_index;

    bool m_hasStarted;
};
