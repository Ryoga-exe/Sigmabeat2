#pragma once
#include "Common.hpp"

namespace Score {

    const String unvalid = U"unvalid";
    constexpr uint32 LevelNum = 4;

    constexpr Color LevelColor[LevelNum] = {
        Color(102, 221, 17 ), // Easy
        Color(255, 170, 1  ), // Normal
        Color(238, 30 , 118), // Hard
        Color(186, 51 , 239)  // Expert
    };
    constexpr Color UnvalidColor = Color(0);

    struct Data {
        String title;
        String artist;
        String category;
        String noteDesigner = U"Anonymous";
        String url;
        FilePath path;
        FilePath parentPath;
        FilePath musicPath;
        FilePath imagePath;
        int32 priority = 0;
        Texture texture;

        uint32 demoStartMs;
        int32 offsetMs;
        double bpm;
        Color backgroundColor[4];
        uint32 level[LevelNum];

        size_t noteStartSeek;
        bool isValid = false;
    };
}