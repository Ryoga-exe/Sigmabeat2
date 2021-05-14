#include "../Common.hpp"
#include "Config.hpp"

namespace Config {
    bool Load(Data& data) {
        const TOMLReader toml(U"config.toml");

        if (!toml) return false;

        data.windowSize = { toml[U"Window.width"].get<int32>(), toml[U"Window.height"].get<int32>() };
        data.windowSizable = toml[U"Window.sizable"].get<bool>();

        for (const auto& object : toml[U"Score.directory"].arrayView())
        {
            data.scoreDirectory << object.get<String>();
        }

        return true;
    }
}