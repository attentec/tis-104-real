#pragma once

#include <stdbool.h>

#include "cpu.h"

struct pipe_t {
    reg_t *cell;
    bool used;
};

void pipe_init(struct pipe_t *pipe);
