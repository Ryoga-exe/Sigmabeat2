#include "Fullscreen.hpp"

namespace Fullscreen {
    static bool isFullscreen, isMaximized, isSizable;
    static Size displaySize, windowSize;
    static Vec2 windowPos;

    void Init(bool sizable) {
        Monitor mainMonitor = System::EnumerateActiveMonitors()[System::GetCurrentMonitorIndex()];
        Rect monitorSize = mainMonitor.displayRect;
        displaySize = { monitorSize.w, monitorSize.h };
        windowSize = Window::ClientSize();
        windowPos = Window::GetState().bounds.pos;
        isFullscreen = isMaximized = false;
        isSizable = sizable;
    }
    bool Set(bool fullscreen) {
        isFullscreen = fullscreen;
        if (fullscreen) {
            if (Window::GetState().maximized) {
                isMaximized = true;
                Window::Restore();
            }
            else isMaximized = false;
            windowSize = Window::ClientSize();
            windowPos = Window::GetState().bounds.pos;
            Window::SetStyle(WindowStyle::Frameless);
            Window::Resize(displaySize);
            Window::SetPos(0, 0);
        }
        else {
            Window::SetStyle(isSizable ? WindowStyle::Sizable : WindowStyle::Fixed);
            Window::Resize(windowSize);
            Window::SetPos(static_cast<int32>(windowPos.x), static_cast<int32>(windowPos.y));
            if (isMaximized) Window::Maximize();
        }
        return fullscreen;
    }
    bool Toggle() {
        return Set(isFullscreen ^ 1);
    }
}