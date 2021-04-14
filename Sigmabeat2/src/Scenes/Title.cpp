#include "Title.hpp"

Title::Title(const InitData& init)
    : IScene(init), m_delta(0.0), m_titleLogo(U"dat/img/logo.png"), m_audio(U"dat/wav/opening.wav", Arg::loop = true) {
    m_audio.play();

}

void Title::update() {
    m_delta += Scene::DeltaTime() * 100;

    if ((KeySpace | KeyEnter).down()) {
        changeScene(SceneState::Menu);
    }
}

void Title::draw() const {

    const double t = m_delta / 75.0;
    auto [x, y] = Scene::Center();
    m_titleLogo.scaled(Window::ClientWidth() / (2100.0 * EaseOutExpo(t))).drawAt(x, y /* - Window::ClientHeight() / 10 * EaseInOutExpo(Max(0.0, t - 1.2)) */ );
    
    {
        const Transformer2D tf2(Mat3x2::Scale(Window::ClientHeight() / 1000.0, { x, y + y / 1.5 }));

        FontAsset(U"Title")(U" - PRESS START KEY TO START - ")
            .drawAt(x, y + y / 1.5, Color(100, (uint32)(((Sin(m_delta / 45.0) + 1.0) / 2.0) * 255)));
    }
    {
        String version = U"1.0.0";
        const Transformer2D tf2(Mat3x2::Scale(Window::ClientHeight() / 1250.0));

        FontAsset(U"Title")(U" Version ", version).draw(Arg::topLeft = Vec2{ 0, 0 }, Color(50));
    }
}