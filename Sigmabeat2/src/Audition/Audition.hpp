#pragma once
#include "Common.hpp"

class Audition {
public:
    Audition(const uint32 index = 0);
    ~Audition();

    bool update(const uint32 index);
    void autoPlayAndStop();
    bool done() const;
    bool stop(const Duration& fadeoutDuration = SecondsF(0.0)) const;

private:
    Audio m_audio;
    uint32 m_index;
    Stopwatch m_stopwatch;

    AsyncTask<Wave> m_task;
    bool m_done, m_changed;
};
