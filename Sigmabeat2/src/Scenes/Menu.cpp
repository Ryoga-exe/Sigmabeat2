#include "Menu.hpp"

Menu::Menu(const InitData& init)
    : IScene(init) {

}

void Menu::update() {

    if (KeyEscape.down()) {
        changeScene(SceneState::Setup);
    }

}

void Menu::draw() const {

}