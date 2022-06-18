#pragma once
#include "Common.hpp"

enum class NoteType {
    Tap              = 0,
    HoldStart        = 1,
    HoldEnd          = 2,
    Press            = 3,
    PassedTap        = 10,
    PassedHoldStart  = 11,
    PassedHoldEnd    = 12,
    PassedPress      = 13,
    PressedHoldStart = 21,

    None             = 255,
};

struct Note {
    NoteType type = NoteType::None;
    uint8 lane = 0;
    int32 timing = 0;
    int32 sub = 0;
};

struct SpeedNote {
    int32 timing = 0;
    double speed = 1.00;
};

struct JudgeYNote {
    int32 timing = 0;
    double posY = 1.00;
    int32 durationMS = 1000;
};
