#include "Audition.hpp"
#include "Score/Manager.hpp"
#include "Base/Singleton.hpp"

namespace {

    Audio CreateAudio(const uint32 index) {
        return Audio(Audio::Stream, Singleton<Score::Manager>::get_instance().getMusicPath(index));
    }
    
}

Audition::Audition(const uint32 index)
    : m_index(index), m_done(false), m_changed(false) {

    m_task = Async(CreateAudio, index);

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

    if (!m_done && m_task.isReady()) {
        if (!m_changed) {
            if (m_audio) {
                m_audio.stop();
                m_audio.release();
            }
            m_audio = m_task.get();
        }
        m_done = true;
    }

    if (m_changed) {
        if (m_done) {
            m_task = Async(CreateAudio, m_index);
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
            m_audio.seekTime(Singleton<Score::Manager>::get_instance().getDemoStartMs(m_index) / 1000.0);
            m_audio.play();
        }
    }

    if (m_stopwatch.isRunning() && m_audio) {
        constexpr int32 fadeInOutMs = 500;
        constexpr int32 length = 29800;
        constexpr int32 span = 200;

        int32 elapsedMs = m_stopwatch.ms();

        if (elapsedMs < fadeInOutMs) {
            m_audio.setVolume((double)elapsedMs / fadeInOutMs);
        }
        else if (elapsedMs < length - fadeInOutMs) {
            m_audio.setVolume(1.0);
        }
        else if (elapsedMs < length) {
            m_audio.setVolume((double)(length - elapsedMs) / fadeInOutMs);
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
