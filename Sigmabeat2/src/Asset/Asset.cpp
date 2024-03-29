﻿#include "Asset.hpp"

namespace Asset {
    void Init() {
        FontAsset::Register(U"Title", 30, Typeface::Regular);
        FontAsset::Register(U"Menu", 60, Typeface::Bold);

        FontAsset::Register(U"Tile.title", 30, Typeface::Medium);
        FontAsset::Register(U"Tile.setting", 40, Typeface::Medium);
        FontAsset::Register(U"Tile.detail", 15, Typeface::Medium);
        FontAsset::Register(U"Tile.detail.small", 10, Typeface::Medium);

        FontAsset::Register(U"Game.combo", 100, Typeface::Bold);
        FontAsset::Register(U"Game.ready", 70, Typeface::Bold);
        FontAsset::Register(U"Game.level", 35, Typeface::Bold);

        FontAsset::Register(U"Game.score", 40, Typeface::Bold);




        TextureAsset::Register(U"Icon.cogs", 0xf085_icon, 100);
        TextureAsset::Register(U"Icon.sliders-h", 0xf1de_icon, 100);

        AudioAsset::Register(U"Opening", Sound::Opening);
        AudioAsset::Register(U"Result", Sound::Result);
        AudioAsset::Register(U"SE.move", Sound::Select);
        AudioAsset::Register(U"SE.levelChange", Sound::LevelChange);
        AudioAsset::Register(U"SE.good", Sound::Good);
        AudioAsset::Register(U"SE.bad", Sound::Bad);
    }
}
