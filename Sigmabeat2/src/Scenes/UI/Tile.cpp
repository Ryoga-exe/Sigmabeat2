#pragma once
#include "Tile.hpp"
#include "Base/Singleton.hpp"

namespace UI {
    namespace Menu {

        Tile::Tile()
            : m_tileRT(TileSize)
            , m_jacketRect(Arg::topCenter = Vec2{ TileSize.x / 2.0, JacketMarginTop }, JacketSize)
            , m_descriptionRect(0, 350, TileSize.x, DescriptionHeight)
            , m_scores(Singleton<Score::Manager>::get_instance()) {

        }

        Tile::~Tile() {

        }

        const MSRenderTexture& Tile::get(size_t index, Color tileColor, double offset) const {

            m_tileRT.clear(Palette::Whitesmoke);

            {
                ScopedRenderTarget2D target(m_tileRT);

                RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.4)).drawFrame(TilePadding, 0.0);

                m_jacketRect.stretched(10).draw(Palette::Dimgray);
                m_jacketRect.stretched(7).drawShadow({ 0, 0 }, 10, TilePadding, Palette::Whitesmoke);
                m_jacketRect(m_scores.getTexture(index)).draw();

                m_descriptionRect.stretched(-TilePadding, 0).drawShadow({ 0, 2 }, 16, 5).draw(Palette::White);

                Line(10, 392, TileSize.x - 10, 392).draw(2.5, ColorF(0.25));


                Rect titleRect = FontAsset(U"Tile.title")(m_scores.getTitle(index)).region();
                Rect artistRect = FontAsset(U"Tile.detail")(m_scores.getArtist(index)).region();

                {
                    Rect descRect = m_descriptionRect.stretched(-10, 0);
                    const ScopedViewport2D descriptionViewport(descRect);

                    if (titleRect.w <= descRect.w) {
                        FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 42), ColorF(0.25));
                    }
                    else {
                        offset = fmod(offset, (titleRect.w + DescriptionOffsetMargin) / DescriptionVel);
                        FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomLeft(descRect.x - offset * DescriptionVel, 42), ColorF(0.25));
                        FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomLeft(descRect.x + titleRect.w + DescriptionOffsetMargin - offset * DescriptionVel, 42), ColorF(0.25));
                    }

                    if (artistRect.w <= descRect.w) {
                        FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 67), ColorF(0.25));
                    }
                    else {
                        offset = fmod(offset, (artistRect.w + DescriptionOffsetMargin) / DescriptionVel);
                        FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomLeft(descRect.x - offset * DescriptionVel, 67), ColorF(0.25));
                        FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomLeft(descRect.x + artistRect.w + DescriptionOffsetMargin - offset * DescriptionVel, 67), ColorF(0.25));
                    }
                }

                FontAsset(U"Tile.detail")(U"NOTES DESIGNER : {}"_fmt(m_scores.getNoteDesigner(index))).draw(Arg::bottomLeft(10, TileSize.y - 5), Palette::White);
                FontAsset(U"Tile.detail")(U"BPM : {}"_fmt(m_scores.getBPM(index))).draw(Arg::bottomRight(TileSize.x - 10, TileSize.y - 5), Palette::White);

                RectF(TileSize).drawFrame(TilePadding, 0.0);

            }

            Graphics2D::Flush();
            m_tileRT.resolve();

            return m_tileRT;
        }

        const MSRenderTexture& Tile::get(const String& type, double number, Color tileColor) const {

            m_tileRT.clear(Palette::Whitesmoke);

            {

                ScopedRenderTarget2D target(m_tileRT);

                RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.4)).drawFrame(TilePadding, 0.0);

                RectF(TileSize).drawFrame(TilePadding, 0.0);

            }

            Graphics2D::Flush();
            m_tileRT.resolve();

            return m_tileRT;
        }

    }
}