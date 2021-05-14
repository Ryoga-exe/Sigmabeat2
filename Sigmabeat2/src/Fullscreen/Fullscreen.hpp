#pragma once
#include "../Common.hpp"

namespace Fullscreen {
    void Init(bool sizable);
    bool Set(bool fullscreen = false);
    bool Toggle();
}