#include "Common.hpp"
#include "Config/Config.hpp"
#include "Base/Singleton.hpp"
#include "Fullscreen/Fullscreen.hpp"
#include "Score/Manager.hpp"
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
    Scene::SetScaleMode(ScaleMode::ResizeFill);
    Scene::SetBackground(Palette::Whitesmoke);

    Fullscreen::Init(data.windowSizable);

    Singleton<Score::Manager>::get_instance().init(data.scoreDirectory);
    Singleton<Score::Manager>::get_instance().load();

    FontAsset::Register(U"Title", 30, Typeface::Regular);
    FontAsset::Register(U"Menu", 60, Typeface::Bold);

    FontAsset::Register(U"Tile.title", 30, Typeface::Medium);
    FontAsset::Register(U"Tile.setting", 40, Typeface::Medium);
    FontAsset::Register(U"Tile.detail", 15, Typeface::Medium);

    TextureAsset::Register(U"Setting", Icon(0xf085, 100));
    TextureAsset::Register(U"Sliders-H", Icon(0xf1de, 100));
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

    //manager.init(SceneState::Setup);
    manager.init(SceneState::Menu);

    while (System::Update() && manager.update()) {

        if (KeyF11.down()) Fullscreen::Toggle();

    }

    Finalize();

}