#include "Result.hpp"

Result::Result(const InitData& init)
    : IScene(init)
    , m_delta(0.0)
    , m_playing(false) {

    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

    AudioAsset::LoadAsync(U"Result");
}

Result::~Result() {
    AudioAsset::Release(U"Result");
}

void Result::update() {
    if (!m_playing && AudioAsset::IsReady(U"Result")) {
        AudioAsset(U"Result").setLoop(true); 
        AudioAsset(U"Result").play();
        m_playing = true;
    }
}

void Result::draw() const {

}
