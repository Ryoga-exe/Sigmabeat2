#pragma once
#include "Common.hpp"
#include "Score/Manager.hpp"
#include "Base/Singleton.hpp"
#include "UI/Tile.hpp"

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
    int32  m_level;
    Stopwatch m_stopwatch;
    Stopwatch m_tileOffsetStopwatch;
    
    UI::Menu::Tile m_tile;
    SizeF  m_tileSize;
    SizeF  m_selectedTileSize;
    SizeF  m_normalTileSize;
    double m_scaleRate;
    double m_selectedTileX;
    double m_tileBaseY;
    double m_animateState;
    double m_tileOffsetXVelocity;
    
};