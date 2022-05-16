#pragma once
#include "Common.hpp"

struct Note {
    uint8 type = 0;
    uint8 lane = 0;
    int32 timing = 0;
    int32 sub = 0;
};

struct SpeedNote {
    int32 timing = 0;
    double speed = 1.00;
};
