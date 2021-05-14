﻿#pragma once
#include "../Common.hpp"

namespace Config {
    struct Data {
        Size windowSize = { 1280, 720 };
        bool windowSizable = true;
        Array<String> scoreDirectory;
    };

    bool Load(Data& data);
}