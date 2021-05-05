#include "Common.hpp"
#include "Fullscreen/Fullscreen.hpp"
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

    Window::SetTitle(U"Sigmabeat");
    Scene::SetBackground(Palette::Whitesmoke);

    Window::Resize(1280, 720);
    Window::Centering();
    Window::SetStyle(WindowStyle::Sizable);
    Scene::SetScaleMode(ScaleMode::ResizeFill);

    Fullscreen::Init();

    Singleton<Score::Manager>::get_instance().init();
    Singleton<Score::Manager>::get_instance().load();

    FontAsset::Register(U"Title", 30, Typeface::Regular);
    AudioAsset::Register(U"Opening", U"dat/wav/opening.wav", AssetParameter::LoadAsync());
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

    //manager.init(SceneState::Menu);
    manager.init(SceneState::Setup);

    // Singleton<Score::Manager>::get_instance().debugPrint(0);
    
    while (System::Update() && manager.update()) {
        
        if (KeyF11.down()) Fullscreen::Toggle();

    }

    Finalize();

}