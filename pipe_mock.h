#pragma once

#include "pipe.h"

struct pipe_t {
    bool value_available;
    bool reader_waiting;
    reg_t value;
};
