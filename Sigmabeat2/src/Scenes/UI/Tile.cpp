#pragma once
#include "Tile.hpp"

namespace UI {
    namespace Menu {

        Tile::Tile()
            : m_tileRT(TileSize) {

        }

        Tile::~Tile() {

        }

        void Tile::draw(Color tileColor) const {
            RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.5)).drawFrame(3.0, 0.0);

        }

    }
}