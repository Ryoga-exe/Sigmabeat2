#pragma once
#include "Common.hpp"
#include "Base/Singleton.hpp"
#include "Score/Manager.hpp"

class Result : public MyApp::Scene {
public:
    Result(const InitData& init);
    ~Result();
    void update() override;
    void draw() const override;

private:
    const String getRank(const int32 scorePoint) const;

    double m_delta = 0.0;
    const Texture m_backgroundTexture;
    Score::Manager& m_scores = Singleton<Score::Manager>::get_instance();
    int32 m_index;
    int32 m_level;
    Array<int32> m_judgeRanks;
    int32 m_scorePoint;
    int32 m_maxCombo;
    MSRenderTexture m_fieldRT;

    bool m_playing;
};
