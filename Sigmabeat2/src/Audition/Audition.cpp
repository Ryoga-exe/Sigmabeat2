#include "Audition.hpp"
#include "Score/Manager.hpp"
#include "Base/Singleton.hpp"

namespace {
    
    Wave CreateWave(const uint32 index) {
        return Wave(Singleton<Score::Manager>::get_instance().getMusicPath(index));
    }
    
}

Audition::Audition(const uint32 index)
    : m_index(index), m_done(false), m_changed(false) {

    m_task = CreateConcurrentTask(CreateWave, index);

}

Audition::~Audition() {
    if (m_audio) {
        m_audio.stop();
        m_audio.release();
    }
}

bool Audition::update(const uint32 index) {
    if (m_index != index) {
        m_changed = true;
        m_index = index;
        if (m_audio) {
            m_audio.stop(0.4s);
            m_stopwatch.reset();
        }
    }

    if (!m_done && m_task.is_done()) {
        if (!m_changed) {
            if (m_audio) {
                m_audio.stop();
                m_audio.release();
            }
            m_audio = Audio(m_task.get());
        }
        m_done = true;
    }

    if (m_changed) {
        if (m_done) {
            m_task = CreateConcurrentTask(CreateWave, m_index);
            m_changed = m_done = false;
        }
    }
    return m_changed;
}

void Audition::autoPlayAndStop() {
    if (m_done && !m_audio.isPlaying()) {
        if (m_audio) {
            m_stopwatch.restart();
            m_audio.setVolume(0.0);
            m_audio.setPosSec(Singleton<Score::Manager>::get_instance().getDemoStartMs(m_index) / 1000.0);
            m_audio.play();
        }
    }

    if (m_stopwatch.isRunning() && m_audio) {
        constexpr int32 introMs = 500;
        constexpr int32 length = 24800;
        constexpr int32 span = 200;

        int32 elapsedMs = m_stopwatch.ms();

        if (elapsedMs < introMs) {
            m_audio.setVolume((double)elapsedMs / introMs);
        }
        else if (elapsedMs < length - introMs) {
            m_audio.setVolume(1.0);
        }
        else if (elapsedMs < length) {
            m_audio.setVolume((double)(length - elapsedMs) / introMs);
        }
        else if (elapsedMs > length + span) {
            m_stopwatch.reset();
            m_audio.stop();
        }
    }
}

bool Audition::done() const {
    return m_done;
}

bool Audition::stop(const Duration& fadeoutDuration) const {
    if (m_done && m_audio) {
        m_audio.stop(fadeoutDuration);
        return true;
    }
    return false;
}