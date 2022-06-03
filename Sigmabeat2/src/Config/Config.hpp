#pragma once
#include "Common.hpp"

namespace Config {
    struct Data {
        Size windowSize = { 1280, 720 };
        bool windowSizable = true;
        bool isArcadeMode = false;
        bool isKeepSceneSize = false;
        Array<FilePath> scoreDirectory;
    };

    bool Load();
    Data& Get();
}
