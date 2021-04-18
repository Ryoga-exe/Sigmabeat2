#include "Common.hpp"
#include "Fullscreen/Fullscreen.hpp"
#include "Singleton/Singleton.hpp"
#include "Scenes/Setup.hpp"
#include "Scenes/Title.hpp"
#include "Scenes/Menu.hpp"
#include "ScoreManager/ScoreManager.hpp"

void Init() {
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);

    Window::SetTitle(U"Sigmabeat");
    Scene::SetBackground(Palette::Whitesmoke);

    Scene::SetScaleMode(ScaleMode::ResizeFill);
    Window::SetStyle(WindowStyle::Sizable);
    Window::Resize(1280, 720);
    Window::Centering();

    Fullscreen::Init();

    Singleton<Score::Manager>::get_instance().init();
    Singleton<Score::Manager>::get_instance().load();

    FontAsset::Register(U"Title", 30, Typeface::Regular);
}

void Finalize() {
    SingletonFinalizer::finalize();
}

void Main() {

    Init();

    MyApp manager;
    manager
        .add<Setup>(SceneState::Setup)
        .add<Title>(SceneState::Title)
        .add<Menu >(SceneState::Menu)
        .setFadeColor(ColorF(1.0));

    manager.init(SceneState::Setup);
    
    while (System::Update() && manager.update()) {

        if (KeyF11.down()) Fullscreen::Toggle();


    }

    Finalize();

}