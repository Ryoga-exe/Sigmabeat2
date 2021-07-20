#pragma once
#include "Common.hpp"

namespace Fullscreen {
    void Init(bool sizable, bool fullscreen = false);
    bool Set(bool fullscreen = false);
    bool Toggle();
}