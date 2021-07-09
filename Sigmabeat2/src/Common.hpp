#pragma once
#define  SIV3D_CONCURRENT // use ConcurrentTask
#include <Siv3D.hpp>      // OpenSiv3D v0.4.3

#define SIGMABEAT_VERSION U"0.2.0 dev"

enum class SceneState {
    Setup,
    Title,
    Menu,
    Game
};

struct GameData {
    int32 selectIndex = 0;
    int32 selectLevel = 0;

    double speed = 2.0;
    double timing = 0.0;
};

using MyApp = SceneManager<SceneState, GameData>;