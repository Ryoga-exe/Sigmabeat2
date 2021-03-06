#pragma once
#include "Common.hpp"
#include "Score/Manager.hpp"

namespace UI {
    constexpr Size   TileSize = { 380, 500 };
    constexpr SizeF  JacketSize = { 270, 270 };
    constexpr double SelectedTileMargin = 70;
    constexpr double TileMargin = 20;
    constexpr double JacketMarginTop = 40.0;
    constexpr double TilePadding = 3.0;
    constexpr double DescriptionHeight = 72;
    constexpr double DescriptionOffsetMargin = 120;
    constexpr double DescriptionVel = 50.0;
        
    class Tile {
    public:
        Tile();
        ~Tile();
            
        const MSRenderTexture& get(size_t index, Color tileColor, double offset = 0.0) const;

        const MSRenderTexture& get(const String& type, double number, Color tileColor) const;

    private:
        MSRenderTexture m_tileRT;
        RectF m_jacketRect, m_descriptionRect;

        Score::Manager& m_scores;
    };
}