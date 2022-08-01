#pragma once
#include <Siv3D.hpp>    // OpenSiv3D v0.6.4
#include "PlayerData/PlayerData.hpp"

#define APPNAME U"Sigmabeat2"
#define SIGMABEAT_VERSION U"0.3.0 dev"
#define CONFIG_FILE U"config.toml"

enum class SceneState {
    Setup,
    Title,
    Menu,
    Game,
    Result,
};

struct GameSettingItem {
    // true_value = value / scale
    int32 value;
    int32 scale;
    int32 minValue;
    int32 maxValue;
};

struct GameData {

    GameData() : judges(5, 0) {}

    PlayerData::Data playerdata;

    int32 selectIndex = 0;
    int32 selectLevel = 0;
    int32 score = 0;
    int32 maxCombo = 0;
    bool  isAuto = false;
    Array<int32> judges;
    
    HashTable<String, GameSettingItem> setting = {
        { U"SPEED",  { 30, 10, 10, 100 } },
        { U"TIMING", { 0, 100, -100, 100 } },
    };
};

using MyApp = SceneManager<SceneState, GameData>;
