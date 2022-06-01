#include "Title.hpp"

Title::Title(const InitData& init)
    : IScene(init), m_delta(0.0), m_titleLogo(U"assets/images/logo.png") {

    AudioAsset(U"Opening").setLoop(true);
    AudioAsset(U"Opening").play();

    Scene::SetBackground(Palette::Whitesmoke);
    Scene::SetLetterbox(Palette::Whitesmoke);

}

Title::~Title() {
    AudioAsset(U"Opening").stop();
    AudioAsset::Release(U"Opening");
}

void Title::update() {
    m_delta += Scene::DeltaTime() * 100;

    if (m_delta >= 100 && !Keyboard::GetAllInputs().empty()) {
        AudioAsset(U"Opening").stop(0.5s);
        changeScene(SceneState::Menu, 1.0s);
    }
}

void Title::draw() const {

    const double t = m_delta / 75.0;
    const auto [x, y] = Scene::Center();
    const auto scaleRate = Min(Scene::Width(), Scene::Height());

    m_titleLogo.scaled(scaleRate / (1100.0 * EaseOutExpo(t))).drawAt(x, y);
    
    {
        const Transformer2D tf2(Mat3x2::Scale(scaleRate / 1000.0, { x, y + y / 1.5 }));

        FontAsset(U"Title")(U" - PRESS ANY KEY TO START - ")
            .drawAt(x, y + y / 1.5, Color(100, (Color::value_type)(((Sin(m_delta / 45.0) + 1.0) / 2.0) * 255)));
    }
    {

        const Transformer2D tf2(Mat3x2::Scale(scaleRate / 1250.0));

        FontAsset(U"Title")(U" Version ", SIGMABEAT_VERSION).draw(Arg::topLeft = Vec2{ 0, 0 }, Color(50));
    }
}
