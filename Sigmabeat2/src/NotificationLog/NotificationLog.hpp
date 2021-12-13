#pragma once
#include "Common.hpp"

namespace NotificationLog {

    struct NotificationData {
        String message;
        Texture icon;
        int32 timeMs;
    };

    bool Add(NotificationData data);
    bool Update();
    void Draw();
}
