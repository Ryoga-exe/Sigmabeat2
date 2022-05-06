#include "Notification.hpp"
#include "Base/Singleton.hpp"

namespace Notification {
    namespace detail {
        class NotificationManager {
        public:
            NotificationManager() {
                
            }

            ~NotificationManager() {

            }

            bool add(NotificationData data) {
                m_data << data;
                // m_data.sort_by([](const NotificationData& a, const NotificationData& b) { return a.timeMs < b.timeMs; });

                return true;
            }

            bool update() {
                bool ret = false;
                for (auto it = m_data.begin(); it != m_data.end();) {
                    if ((*it).timeMs <= 0) {
                        it = m_data.erase(it);
                        ret = true;
                    }
                    else {
                        ++it;
                    }
                }
                return ret;
            }

            void draw() {
                
            }

        private:
            Array<NotificationData> m_data;
        };
    }

    bool Add(NotificationData data) {
        return Singleton<detail::NotificationManager>::get_instance().add(data);
    }

    bool Update() {
        return Singleton<detail::NotificationManager>::get_instance().update();
    }

    void Draw() {
        Singleton<detail::NotificationManager>::get_instance().draw();
    }
}
