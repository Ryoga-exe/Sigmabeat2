﻿#include "Setup.hpp"
#include "Config/Config.hpp"

Setup::Setup(const InitData& init)
    : IScene(init), m_stopwatch(StartImmediately::Yes), m_authorLogo(U"assets/images/ryoga-exe.png"){
    AudioAsset::LoadAsync(U"Opening");

    Scene::SetBackground(Palette::Whitesmoke);
    if (!Config::Get().keepSceneSize) {
        Scene::SetLetterbox(Palette::Whitesmoke);
    }

}

void Setup::update() {
    if (m_stopwatch.elapsed() > 2.0s && AudioAsset::IsReady(U"Opening")) {
        changeScene(SceneState::Title);
    }
}

void Setup::draw() const {
    const ScopedRenderStates2D state(SamplerState::ClampNearest);
    m_authorLogo.scaled(Min(Scene::Width(), Scene::Height()) / 130.0).drawAt(Scene::Center());
}
