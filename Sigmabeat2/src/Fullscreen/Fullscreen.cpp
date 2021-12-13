#include "Fullscreen.hpp"
#include "Base/Singleton.hpp"

namespace Fullscreen {
    namespace detail {
        class FullscreenManager {
        public:
            FullscreenManager()
                : m_isFullscreen(false), m_isMaximized(false), m_isSizable(false),
                  m_displaySize(), m_windowPos(), m_windowSize() {}

            void Init(bool sizable, bool fullscreen) {
                const Array<MonitorInfo> monitors = System::EnumerateMonitors();
                const Rect monitorSize = monitors[System::GetCurrentMonitorIndex()].displayRect;
                m_displaySize = monitorSize.size;
                m_windowSize = Window::GetState().virtualSize;
                m_windowPos = Window::GetState().bounds.pos;
                m_isMaximized = false;
                m_isSizable = sizable;

                if (fullscreen) {
                    Set(true);
                }
            }

            bool Set(bool fullscreen) {
                m_isFullscreen = fullscreen;
                if (fullscreen) {
                    if (Window::GetState().maximized) {
                        m_isMaximized = true;
                        Window::Restore();
                    }
                    else {
                        m_isMaximized = false;
                    }
                    m_windowSize = Window::GetState().virtualSize;
                    m_windowPos = Window::GetState().bounds.pos;
                    Window::SetStyle(WindowStyle::Frameless);
                    Window::Resize(m_displaySize);
                    Window::SetPos(0, 0);
                }
                else {
                    Window::SetStyle(m_isSizable ? WindowStyle::Sizable : WindowStyle::Fixed);
                    Window::Resize(m_windowSize);
                    Window::SetPos(static_cast<int32>(m_windowPos.x), static_cast<int32>(m_windowPos.y));
                    if (m_isMaximized) {
                        Window::Maximize();
                    }
                }
                return fullscreen;
            }

            bool Toggle() {
                return Set(m_isFullscreen ^ 1);
            }

        private:
            bool m_isFullscreen, m_isMaximized, m_isSizable;
            Size m_displaySize, m_windowSize;
            Vec2 m_windowPos;
        };
    }

    void Init(bool sizable, bool fullscreen) {
        Singleton<detail::FullscreenManager>::get_instance().Init(sizable, fullscreen);
    }

    bool Set(bool fullscreen) {
        return Singleton<detail::FullscreenManager>::get_instance().Set(fullscreen);
    }

    bool Toggle() {
        return Singleton<detail::FullscreenManager>::get_instance().Toggle();
    }
}
