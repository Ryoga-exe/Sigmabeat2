#pragma once
#include <Siv3D.hpp> // OpenSiv3D v0.4.3

enum class SceneState {
    Setup,
    Title,
    Menu
};

struct GameData {

};

using MyApp = SceneManager<SceneState, GameData>;