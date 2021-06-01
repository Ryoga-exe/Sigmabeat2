#pragma once
#include "Common.hpp"

namespace UI {
    namespace Menu {

        constexpr double SelectedTileSize = 270;
        constexpr double NormalIndexSize = 200;
        constexpr double SizeBetween = SelectedTileSize - NormalIndexSize;
        constexpr double MarginSize = NormalIndexSize / 2.5;
        constexpr double SelectedTileMarginSize = 50;

        constexpr Size TileSize = { 350, 470 };

        class Tile {
        public:
            Tile();
            ~Tile();

            void draw(Color tileColor) const;

        private:
            RenderTexture m_tileRT;
        };
    }
}