#include "Common.hpp"
#include "Fullscreen/Fullscreen.hpp"
#include "Config/Config.hpp"
#include "Base/Singleton.hpp"
#include "Scenes/Setup.hpp"
#include "Scenes/Title.hpp"
#include "Scenes/Menu.hpp"
#include "ScoreManager/ScoreManager.hpp"

void Init() {
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);
# if SIV3D_PLATFORM(WINDOWS)
    Platform::Windows::TextInput::DisableIME();
#endif

    Config::Data data;
    Config::Load(data);

    Window::SetTitle(U"Sigmabeat");
    Window::Resize(data.windowSize);
    Window::SetStyle(data.windowSizable ? WindowStyle::Sizable : WindowStyle::Fixed);
    Scene::SetScaleMode(ScaleMode::ResizeFill);
    Scene::SetBackground(Palette::Whitesmoke);

    Fullscreen::Init(data.windowSizable);

    Singleton<Score::Manager>::get_instance().init(data.scoreDirectory);
    Singleton<Score::Manager>::get_instance().load();

    FontAsset::Register(U"Title", 30, Typeface::Regular);
    FontAsset::Register(U"Menu", 60, Typeface::Bold);
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