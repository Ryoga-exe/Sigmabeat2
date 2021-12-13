#include "Common.hpp"
#include "Config/Config.hpp"
#include "Base/Singleton.hpp"
#include "Fullscreen/Fullscreen.hpp"
#include "Score/Manager.hpp"
#include "Asset/Asset.hpp"
#include "Scenes/Setup.hpp"
#include "Scenes/Title.hpp"
#include "Scenes/Menu.hpp"
#include "Scenes/Game.hpp"

void Init() {
    System::SetTerminationTriggers(UserAction::CloseButtonClicked);
# if SIV3D_PLATFORM(WINDOWS)
    Platform::Windows::TextInput::DisableIME();
#endif

    Config::Load();
    Config::Data& data = Config::Get();

    Window::SetTitle(U"Sigmabeat");
    Window::Resize(data.windowSize);
    Window::SetStyle(data.windowSizable ? WindowStyle::Sizable : WindowStyle::Fixed);

    Scene::SetBackground(Palette::Whitesmoke);

    Fullscreen::Init(data.windowSizable);
    Asset::Init();

    Singleton<Score::Manager>::get_instance().init(data.scoreDirectory);
    Singleton<Score::Manager>::get_instance().load();

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
        .add<Game >(SceneState::Game)
        .setFadeColor(ColorF(1.0));

    manager.init(SceneState::Setup);
    //manager.init(SceneState::Menu);

    while (System::Update() && manager.update()) {

        if (KeyF11.down()) {
            Fullscreen::Toggle();
        }

    }

    Finalize();

}
