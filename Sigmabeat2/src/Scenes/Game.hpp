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
    constexpr Color JudmentLineBlurColor(Palette::Red);

}

class Game : public MyApp::Scene {
public:
    Game(const InitData& init);
    ~Game();
    void update() override;
    void draw() const override;


private:
    void drawField() const;
    void drawNote(Note note) const;
    void drawJudmentLine() const;
    bool loadNotes();
    double calculateNoteY(int32 timing, double speed) const;

    struct Homography {
        Float4 m1;
        Float4 m2;
        Float4 m3;
    };

    Score::Manager& m_scores = Singleton<Score::Manager>::get_instance();

    const Size FieldSize = { 500, 1000 };
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

    double m_judgementYPos;
    double m_laneWidth;
    double m_speed;
    int32 m_combo;
    int32 m_endTime;
    int32 m_speedMapIndex;
    int32 m_index;

    bool m_hasStarted;
};
