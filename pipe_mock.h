#pragma once

#include "pipe.h"

struct pipe_t {
    bool value_available;
    bool value_read;
    reg_t value;
};
