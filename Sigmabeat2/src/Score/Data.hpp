#pragma once
#include "Common.hpp"
#include "UI/Color.hpp"

namespace Score {

    const String unvalid = U"unvalid";
    constexpr uint32 LevelNum = 4;

    constexpr Color LevelColor[LevelNum] = {
        UI::EasyColor,
        UI::NormalColor,
        UI::HardColor,
        UI::ExpertColor
    };
    const String LevelName[LevelNum] = {
        U"Easy",
        U"Normal",
        U"Hard",
        U"Expert",
    };
    constexpr Color UnvalidLevelColor = UI::UnvalidLevelColor;
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
