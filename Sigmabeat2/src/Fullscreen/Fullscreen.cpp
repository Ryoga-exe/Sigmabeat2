#include "../Common.hpp"
#include "Fullscreen.hpp"

namespace Fullscreen {
    static bool isFullscreen;
    static Size displaySize, windowSize;

    void Init() {
        Monitor mainMonitor = System::EnumerateActiveMonitors()[System::GetCurrentMonitorIndex()];
        Rect monitorSize = mainMonitor.displayRect;
        displaySize = { monitorSize.w, monitorSize.h };
        windowSize = Window::ClientSize();
        isFullscreen = false;
    }
    bool Set(bool fullscreen) {
        isFullscreen = fullscreen;
        if (fullscreen) {
            windowSize = Window::ClientSize();
            Window::SetStyle(WindowStyle::Frameless);
            Window::Resize(displaySize);
            Window::SetPos(0, 0);
        }
        else {
            Window::SetStyle(WindowStyle::Sizable);
            Window::Resize(windowSize);
            Window::Centering();
        }
        return fullscreen;
    }
    bool Toggle() {
        return Set(isFullscreen ^ 1);
    }
}