﻿#pragma once
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

        void Tile::draw(size_t index, Color tileColor, double offset) const {

            const ScopedViewport2D tileViewport(TileSize);

            RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.5)).drawFrame(TilePadding, 0.0);

            m_jacketRect.stretched(10).draw(Palette::Dimgray);
            m_jacketRect.stretched(7).drawShadow({ 0, 0 }, 10, TilePadding, Palette::Whitesmoke);
            m_jacketRect(m_scores.getTexture(index)).draw();

            m_descriptionRect.stretched(-TilePadding, 0).drawShadow({ 0, 2 }, 16, 5).draw(Palette::White);

            Line(10, 392, TileSize.x - 10, 392).draw(2.5, ColorF(0.25));


            Rect titleRect  = FontAsset(U"Tile.title")(m_scores.getTitle(index)).region();
            Rect artistRect = FontAsset(U"Tile.detail")(m_scores.getArtist(index)).region();

            {
                Rect descRect = m_descriptionRect.stretched(-10, 0);
                const ScopedViewport2D descriptionViewport(descRect);

                if (titleRect.w <= descRect.w) {
                    FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 42), ColorF(0.25));
                }
                else {
                    offset = fmod(offset, (titleRect.w + DescriptionOffsetMargin) / 30.0);
                    FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomLeft(descRect.x - offset * 30, 42), ColorF(0.25));
                    FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomLeft(descRect.x + titleRect.w + DescriptionOffsetMargin - offset * 30, 42), ColorF(0.25));
                }

                if (artistRect.w <= descRect.w) {
                    FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 67), ColorF(0.25));
                }
                else {
                    offset = fmod(offset, (artistRect.w + DescriptionOffsetMargin) / 30.0);
                    FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomLeft(descRect.x - offset * 30, 67), ColorF(0.25));
                    FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomLeft(descRect.x + artistRect.w + DescriptionOffsetMargin - offset * 30, 67), ColorF(0.25));
                }
            }
            
            String designer = U"Sample";

            FontAsset(U"Tile.detail")(U"NOTES DESIGNER : " + designer).draw(Arg::bottomLeft(10, TileSize.y - 5), Palette::White);
            FontAsset(U"Tile.detail")(U"BPM : " + Format(m_scores.getBPM(index))).draw(Arg::bottomRight(TileSize.x - 10, TileSize.y - 5), Palette::White);

            RectF(TileSize).drawFrame(TilePadding, 0.0);

        }

    }
}