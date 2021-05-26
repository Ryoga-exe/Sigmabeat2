#pragma once
#include "../Common.hpp"
#include "../ScoreManager/ScoreManager.hpp"
#include "../Base/Singleton.hpp"

class Menu : public MyApp::Scene {
public:
    Menu(const InitData& init);
    void update() override;
    void draw() const override;

private:
    void updateTiles();
    void drawTiles() const;

    Score::Manager& m_scores = Singleton<Score::Manager>::get_instance();

    int32  m_indexSize;
    int32  m_selectedIndex;

    Stopwatch m_stopwatch;
    
    double m_selectedTileSize;
    double m_tileSize;
    double m_tileMargin;
    double m_selectedTileX;
    double m_tileBaseY;

    double m_animateState;
    double m_tileOffsetXVelocity;
    
};