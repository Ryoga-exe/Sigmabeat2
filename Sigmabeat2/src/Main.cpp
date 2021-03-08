#include "Common.hpp"
#include "Fullscreen/Fullscreen.hpp"

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

    while (System::Update()) {

        if (KeyF11.down()) {
            Fullscreen::Toggle();
        }
    }
}