#include "Asset.hpp"

namespace Asset {
    void Init() {
        FontAsset::Register(U"Title", 30, Typeface::Regular);
        FontAsset::Register(U"Menu", 60, Typeface::Bold);

        FontAsset::Register(U"Tile.title", 30, Typeface::Medium);
        FontAsset::Register(U"Tile.setting", 40, Typeface::Medium);
        FontAsset::Register(U"Tile.detail", 15, Typeface::Medium);

        TextureAsset::Register(U"Icon.cogs", Icon(0xf085, 100));
        TextureAsset::Register(U"Icon.sliders-h", Icon(0xf1de, 100));
    }
}