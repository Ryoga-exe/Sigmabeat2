#pragma once
#include "Tile.hpp"

namespace UI {
    namespace Menu {

        Tile::Tile()
            : m_tileRT(TileSize), m_jacketRect(Arg::topCenter = Vec2{ TileSize.x / 2.0, JacketMarginTop }, JacketSize) {

        }

        Tile::~Tile() {

        }

        void Tile::draw(Color tileColor) const {
            RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.5)).drawFrame(3.0, 0.0);

            m_jacketRect.stretched(10).draw(Palette::Dimgray);
            m_jacketRect.stretched(7).drawShadow({ 0.0, 0.0 }, 10, 3.0, Palette::Whitesmoke);
            m_jacketRect.draw(Palette::Black);

            RectF(0, 350, 380, 70).draw(Palette::White);

            RectF(10, 390, 360, 2).draw(ColorF(0.25));

            String title = U"Sample Title";
            FontAsset(U"Tile.title")(title).draw(Arg::bottomCenter(190, 390), ColorF(0.25));
            FontAsset(U"Tile.artist")(title).draw(Arg::bottomCenter(190, 420), ColorF(0.25));
            //FontAsset(U"Menu")(U"MUSIC SELECT").draw(Arg::topCenter = Point{ Scene::Center().x , 0 }, Palette::Black);

        }

    }
}