#include "../Common.hpp"
#include "Config.hpp"

namespace Config {
    bool Load(Data& data) {
        const TOMLReader toml(U"config.toml");

        if (!toml) return false;



        return true;
    }
}