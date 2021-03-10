#include "Common.hpp"
#include "Fullscreen/Fullscreen.hpp"
#include "Scenes/Setup.hpp"
#include "Scenes/Title.hpp"

void Init() {
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);

    Window::SetTitle(U"Sigmabeat");
    Scene::SetBackground(Palette::Whitesmoke);

    Scene::SetScaleMode(ScaleMode::ResizeFill);
    Window::SetStyle(WindowStyle::Sizable);
    Window::Resize(1280, 720);
    Window::Centering();

    Fullscreen::Init();
}

void Main() {

    Init();

    MyApp manager;
    manager
        .add<Setup>(SceneState::Setup)
        .add<Title>(SceneState::Title)
        .setFadeColor(ColorF(1.0));

    manager.init(SceneState::Setup);
    
    while (System::Update() && manager.update()) {

        if (KeyF11.down()) Fullscreen::Toggle();


    }
}