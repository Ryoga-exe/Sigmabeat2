#pragma once
#include "Common.hpp"

namespace Notification {

    struct NotificationData {
        String message;
        Texture icon;
        int32 timeMs;
    };

    bool Add(NotificationData data);
    bool Update();
    void Draw();
}
