#pragma once
#include "Common.hpp"

namespace Asset {
    constexpr FilePathView OpeningWav = U"assets/sounds/BGM/opening.wav";
    constexpr FilePathView ResultWav = U"assets/sounds/BGM/result.wav";
    constexpr FilePathView Select_SE = U"assets/sounds/SE/Select_SE.wav";
    constexpr FilePathView LevelChange_SE = U"assets/sounds/SE/LevelChange_SE.ogg";

    namespace Image {
        constexpr FilePathView Logo = U"assets/images/logos/sigmabeat.png";
        constexpr FilePathView Ryoga_exe = U"assets/images/logos/ryoga-exe.png";
    }
    namespace Shader {
        const FilePath HomographyHLSL = U"assets/shaders/hlsl/homography.hlsl";
        const FilePath HomographyVERT = U"assets/shaders/glsl/homography.vert";
        const FilePath HomographyFRAG = U"assets/shaders/glsl/homography.frag";
    }

    void Init();
}
