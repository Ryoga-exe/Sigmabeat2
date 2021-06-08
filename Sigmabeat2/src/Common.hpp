#pragma once
#include <Siv3D.hpp> // OpenSiv3D v0.4.3

#define SIGMABEAT_VERSION U"0.2.0 dev"

enum class SceneState {
    Setup,
    Title,
    Menu
};

struct GameData {

};

using MyApp = SceneManager<SceneState, GameData>;