#include "Setup.hpp"

Setup::Setup(const InitData& init)
    : IScene(init), m_stopwatch(true), m_authorLogo(U"dat/img/ryoga-exe.png"){
}

void Setup::update() {
    if (m_stopwatch.elapsed() > 2.0s) {
        changeScene(SceneState::Title);
    }
}

void Setup::draw() const {
    const ScopedRenderStates2D state(SamplerState::ClampNearest);
    m_authorLogo.scaled(Min(Scene::Width(), Scene::Height()) / 130.0).drawAt(Scene::Center());
}