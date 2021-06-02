#pragma once
#include "Common.hpp"

namespace UI {
    namespace Menu {

        constexpr double SelectedTileSize = 270;
        constexpr double NormalIndexSize = 200;
        constexpr double SizeBetween = SelectedTileSize - NormalIndexSize;
        constexpr double MarginSize = NormalIndexSize / 2.5;
        constexpr double SelectedTileMarginSize = 50;

        constexpr Size TileSize = { 380, 500 };
        constexpr Size JacketSize = { 270, 270 };
        constexpr double JacketMarginTop = 40.0;

        class Tile {
        public:
            Tile();
            ~Tile();

            void draw(Color tileColor) const;

        private:
            RenderTexture m_tileRT;
            RectF m_jacketRect;
        };
    }
}