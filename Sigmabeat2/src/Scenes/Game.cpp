#include "Game.hpp"

Game::Game(const InitData& init)
    : IScene(init) {

    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

}

void Game::update() {

}

void Game::draw() const {

}
