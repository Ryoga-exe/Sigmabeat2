#include "Asset.hpp"

namespace Asset {
    void Init() {
        FontAsset::Register(U"Title", 30, Typeface::Regular);
        FontAsset::Register(U"Menu", 60, Typeface::Bold);

        FontAsset::Register(U"Tile.title", 30, Typeface::Medium);
        FontAsset::Register(U"Tile.setting", 40, Typeface::Medium);
        FontAsset::Register(U"Tile.detail", 15, Typeface::Medium);

        FontAsset::Register(U"Game.combo", 100, Typeface::Bold);



        TextureAsset::Register(U"Icon.cogs", 0xf085_icon, 100);
        TextureAsset::Register(U"Icon.sliders-h", 0xf1de_icon, 100);

        AudioAsset::Register(U"Opening", U"assets/sounds/opening.wav");
        AudioAsset::Register(U"Result", U"assets/sounds/result.wav");
        AudioAsset::Register(U"SE.move", U"assets/sounds/Select_SE.wav");
        AudioAsset::Register(U"SE.levelChange", U"assets/sounds/LevelChange_SE.ogg");
    }
}
