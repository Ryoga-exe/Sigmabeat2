﻿#include "Tile.hpp"
#include "Base/Singleton.hpp"

namespace UI {
    Tile::Tile()
        : m_tileRT(TileSize)
        , m_jacketRect(Arg::topCenter = Vec2{ TileSize.x / 2.0, JacketMarginTop }, JacketSize)
        , m_descriptionRect(0, 350, TileSize.x, DescriptionHeight)
        , m_scores(Singleton<Score::Manager>::get_instance()) {

    }

    Tile::~Tile() {

    }

    const MSRenderTexture& Tile::get(size_t index, size_t level, double offset) const {

        m_tileRT.clear(Palette::Whitesmoke);
        Color tileColor = Score::LevelColor[level];
        if (m_scores.get(index).level[level] == 0) {
            tileColor = Score::UnvalidLevelColor;
        }

        {
            ScopedRenderTarget2D target(m_tileRT);

            RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.4)).drawFrame(TilePadding, 0.0);

            m_jacketRect.stretched(10).draw(Palette::Dimgray);
            m_jacketRect.stretched(7).drawShadow({ 0, 0 }, 10, TilePadding, Palette::Whitesmoke);
            m_jacketRect(m_scores.getTexture(index)).draw();

            m_descriptionRect.stretched(-TilePadding, 0).drawShadow({ 0, 2 }, 16, 5).draw(Palette::White);

            Line(10, 392, TileSize.x - 10, 392).draw(2.5, ColorF(0.25));


            RectF titleRect = FontAsset(U"Tile.title")(m_scores.getTitle(index)).region();
            RectF artistRect = FontAsset(U"Tile.detail")(m_scores.getArtist(index)).region();

            {
                Rect descRect = m_descriptionRect.stretched(-10, 0).asRect();
                const ScopedViewport2D descriptionViewport(descRect);

                if (titleRect.w <= descRect.w) {
                    FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 42), ColorF(0.25));
                }
                else {
                    offset = Fmod(offset, (titleRect.w + DescriptionOffsetMargin) / DescriptionVel);
                    FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomLeft(descRect.x - offset * DescriptionVel, 42), ColorF(0.25));
                    FontAsset(U"Tile.title")(m_scores.getTitle(index)).draw(Arg::bottomLeft(descRect.x + titleRect.w + DescriptionOffsetMargin - offset * DescriptionVel, 42), ColorF(0.25));
                }

                if (artistRect.w <= descRect.w) {
                    FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 67), ColorF(0.25));
                }
                else {
                    offset = Fmod(offset, (artistRect.w + DescriptionOffsetMargin) / DescriptionVel);
                    FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomLeft(descRect.x - offset * DescriptionVel, 67), ColorF(0.25));
                    FontAsset(U"Tile.detail")(m_scores.getArtist(index)).draw(Arg::bottomLeft(descRect.x + artistRect.w + DescriptionOffsetMargin - offset * DescriptionVel, 67), ColorF(0.25));
                }
            }

            RectF{ 15, 290, 65, 65 }.drawShadow({ 0, 2 }, 13, 2).draw(ColorF(0, 0, 0, 0.3)).draw(Palette::Whitesmoke);
            RectF{ 18, 310, 59, 42 }.draw(Color(15, 51, 66));
            FontAsset(U"Tile.detail")(U"LEVEL").draw(Arg::bottomCenter(48.0, 310), Color(15, 51, 66));
            if (m_scores.get(index).level[level] == 0) {
                FontAsset(U"Tile.title")(U"-").draw(Arg::center(47.5, 331), Palette::White);
            }
            else {
                FontAsset(U"Tile.title")(m_scores.get(index).level[level]).draw(Arg::center(47.5, 331), Palette::White);
            }

            FontAsset(U"Tile.detail")(U"NOTES DESIGNER : {}"_fmt(m_scores.getNoteDesigner(index))).draw(Arg::bottomLeft(10, TileSize.y - 5), Palette::White);
            FontAsset(U"Tile.detail")(U"BPM : {:.0f}"_fmt(m_scores.getBPM(index))).draw(Arg::bottomRight(TileSize.x - 10, TileSize.y - 5), Palette::White);

            RectF(TileSize).drawFrame(TilePadding, 0.0);

        }

        Graphics2D::Flush();
        m_tileRT.resolve();

        return m_tileRT;
    }

    const MSRenderTexture& Tile::get(const String& type, GameSettingItem item, Color tileColor) const {

        String message;

        if (type == U"SPEED") message = U"スピード設定";
        else if (type == U"TIMING") message = U"タイミング設定";


        m_tileRT.clear(Palette::Whitesmoke);

        {

            ScopedRenderTarget2D target(m_tileRT);

            RectF(TileSize).draw(Arg::top = tileColor, Arg::bottom = ColorF(tileColor).gamma(0.4)).drawFrame(TilePadding, 0.0);

            Line(20, 20, 20, 75).draw(3.0, ColorF(0.95));
            FontAsset(U"Tile.title")(type).draw(Vec2{ 27, 10 }, ColorF(0.95));
            FontAsset(U"Tile.title")(U"SETTING").draw(Vec2{ 27, 40 }, ColorF(0.95));

            Rect descRect = m_descriptionRect.stretched(-10, 0).asRect();
            FontAsset(U"Tile.setting")(message).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 200), ColorF(0.95));
            Line(25, 202, TileSize.x - 25, 202).draw(3.0, ColorF(0.95));

            RectF(TileSize).movedBy(0, 300).drawShadow({ 0, 0 }, 16, 2).draw(Arg::top = Palette::Darkgray, Arg::bottom = Palette::Black);

            RectF(descRect.stretched(-40, 15)).drawShadow({ 0, 0 }, 16, 2).draw(ColorF(0.95));

            double value = item.value / (double)item.scale;
            if (type == U"SPEED") {
                FontAsset(U"Tile.setting")(U"x{:.1f}"_fmt(value)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 400), ColorF(0.25));
                TextureAsset(U"Icon.cogs").draw(Vec2{ 230, 20 }, ColorF(0, 0.3));
            }
            else if (type == U"TIMING") {
                FontAsset(U"Tile.setting")(item.value == 0.00 ? U"±0.00" : U"{:+.2f}"_fmt(value)).draw(Arg::bottomCenter(descRect.x + descRect.w / 2.0, 400), ColorF(0.25));
                TextureAsset(U"Icon.sliders-h").draw(Vec2{ 255, 20 }, ColorF(0, 0.3));
            }

            FontAsset(U"Tile.detail")(U"上下キーで変更").draw(Arg::topCenter(descRect.x + descRect.w / 2.0, 400), ColorF(0.25));
            RectF(TileSize).drawFrame(TilePadding, 0.0);

        }

        Graphics2D::Flush();
        m_tileRT.resolve();

        return m_tileRT;
    }
}
