#include <stddef.h>

#include "panic.h"
#include "pipe.h"
#include "pipe_mock.h"

void pipe_init(struct pipe_t *pipe)
{
    pipe->cell = NULL;
    pipe->used = false;
}

bool input_accept(struct pipe_t *input, reg_t *value) {
    if (input == NULL) {
        return false;
    }
    if (input->cell && *(input->cell) != REG_INVALID_VALUE) {
        *value = *(input->cell);
        *(input->cell) = REG_INVALID_VALUE;
        input->used = true;
        return true;
    } else {
        return false;
    }
}

bool output_used(struct pipe_t *output) {
    if (output == NULL) {
        return false;
    }
    return output->used;
}

bool output_taken(struct pipe_t *output) {
    if (output == NULL) {
        return false;
    }
    if (output->cell == NULL) {
        panic();
    }
    return *(output->cell) == REG_INVALID_VALUE;
}

void output_offer(struct pipe_t *output, reg_t *cell) {
    if (output == NULL) {
        return;
    }
    output->used = false;
    output->cell = cell;
}

void output_abstain(struct pipe_t *output) {
    if (output == NULL) {
        return;
    }
    output->cell = NULL;
}
