#pragma once
#include "Common.hpp"

namespace Config {
    struct Data {
        Size windowSize = { 1280, 720 };
        bool windowSizable = true;
        bool isArcadeMode = false;
        bool keepSceneSize = false;
        Color letterboxColor = Palette::Black;
        Array<FilePath> scoreDirectories;
    };

    bool Load();
    Data& Get();
}
