#pragma once
#include "Common.hpp"

namespace Asset {
    constexpr StringView OpeningWav = U"assets/sounds/BGM/opening.wav";
    constexpr StringView ResultWav = U"assets/sounds/BGM/result.wav";
    constexpr StringView Select_SE = U"assets/sounds/SE/Select_SE.wav";
    constexpr StringView LevelChange_SE = U"assets/sounds/SE/LevelChange_SE.ogg";

    namespace Image {
        constexpr StringView Logo = U"assets/images/logos/sigmabeat.png";
        constexpr StringView Ryoga_exe = U"assets/images/logos/ryoga-exe.png";
    }

    void Init();
}
