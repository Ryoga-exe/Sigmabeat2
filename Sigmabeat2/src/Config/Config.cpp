#include "Config.hpp"
#include "Base/Singleton.hpp"

namespace Config {
    namespace detail {
        class ConfigManager {
        public:
            ConfigManager() {}

            bool load() {
                const TOMLReader toml(U"config.toml");

                if (!toml) return false;

                m_data.windowSize = { toml[U"Window.width"].get<int32>(), toml[U"Window.height"].get<int32>() };
                m_data.windowSizable = toml[U"Window.sizable"].get<bool>();
                m_data.isKeepSceneSize = toml[U"Window.keepScene"].get<bool>();
                m_data.isArcadeMode = toml[U"System.arcade"].get<bool>();

                for (const auto& object : toml[U"Score.directory"].arrayView()) {
                    m_data.scoreDirectory << object.get<FilePath>();
                }

                return true;
            }

            Data& get() {
                return m_data;
            }

        private:
            Data m_data;

        };
    }


    bool Load() {
        return Singleton<detail::ConfigManager>::get_instance().load();
    }

    Data& Get() {
        return Singleton<detail::ConfigManager>::get_instance().get();
    }
}
