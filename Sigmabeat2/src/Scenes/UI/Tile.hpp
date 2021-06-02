#pragma once
#include "Common.hpp"
#include "Score/Manager.hpp"

namespace UI {
    namespace Menu {

        constexpr double SelectedTileSize = 270;
        constexpr double NormalIndexSize = 200;
        constexpr double SizeBetween = SelectedTileSize - NormalIndexSize;
        constexpr double MarginSize = NormalIndexSize / 1.5;
        constexpr double SelectedTileMarginSize = 50;

        constexpr Size TileSize = { 380, 500 };
        constexpr Size JacketSize = { 270, 270 };
        constexpr double JacketMarginTop = 40.0;
        constexpr double TilePadding = 3.0;
        constexpr double DescriptionHeight = 72;
        constexpr double DescriptionOffsetMargin = 120;

        class Tile {
        public:
            Tile();
            ~Tile();

            void draw(size_t index, Color tileColor, double offset = 0.0) const;

        private:
            RenderTexture m_tileRT;
            RectF m_jacketRect, m_descriptionRect;

            Score::Manager& m_scores;
        };
    }
}