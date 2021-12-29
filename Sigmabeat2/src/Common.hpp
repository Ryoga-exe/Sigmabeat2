#pragma once
#include <Siv3D.hpp>      // OpenSiv3D v0.6.3
#include "PlayerData/PlayerData.hpp"

#define SIGMABEAT_VERSION U"0.2.0 dev"

enum class SceneState {
    Setup,
    Title,
    Menu,
    Game
};

struct GameData {

    PlayerData::Data playerdata;

    int32 selectIndex = 0;
    int32 selectLevel = 0;
    
    HashTable<String, std::pair<double, double>> setting = {
        { U"SPEED",  { 2.0, 0.1  } },
        { U"TIMING", { 0.0, 0.01 } },
    };
};

using MyApp = SceneManager<SceneState, GameData>;
