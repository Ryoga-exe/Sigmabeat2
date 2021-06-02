#pragma once
#include "Tile.hpp"
#include "Base/Singleton.hpp"

namespace UI {
    namespace Menu {

        Tile::Tile()
            : m_tileRT(TileSize)
            , m_jacketRect(Arg::topCenter = Vec2{ TileSize.x / 2.0, JacketMarginTop }, JacketSize)
            , m_scores(Singleton<Score::Manager>::get_instance()) {

        }

        Tile::~Tile() {

        }

        void Tile::draw(size_t index, Color tileColor) const {
            RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.5)).drawFrame(3.0, 0.0);

            m_jacketRect.stretched(10).draw(Palette::Dimgray);
            m_jacketRect.stretched(7).drawShadow({ 0.0, 0.0 }, 10, 3.0, Palette::Whitesmoke);
            m_jacketRect(m_scores.getTexture(index)).draw();

            RectF(3, 350, 374, 72).drawShadow({ 0, 5 }, 16, 0).draw(Palette::White);

            RectF(10, 392, 360, 2).draw(ColorF(0.25));

            String designer = U"Sample";
            FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomCenter(190, 392), ColorF(0.25));
            FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomCenter(190, 417), ColorF(0.25));

            FontAsset(U"Tile.detail")(U"NOTES DESIGNER : " + designer).draw(Arg::bottomLeft(10, TileSize.y - 5), Palette::White);
            FontAsset(U"Tile.detail")(U"BPM : " + Format(m_scores.getBPM(index))).draw(Arg::bottomRight(TileSize.x - 10, TileSize.y - 5), Palette::White);

        }

    }
}