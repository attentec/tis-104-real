#include <stddef.h>

#include "pipe.h"
#include "pipe_mock.h"
#include "panic.h"

bool input_accept(struct pipe_t *input, reg_t *value) {
    if (input->cell && *(input->cell) != REG_INVALID_VALUE) {
        *value = *(input->cell);
        *(input->cell) = REG_INVALID_VALUE;
        return true;
    } else {
        return false;
    }
}

bool output_taken(struct pipe_t *output) {
    if (output->cell == NULL) {
        panic();
    }
    return *(output->cell) == REG_INVALID_VALUE;
}

void output_offer(struct pipe_t *output, reg_t *cell) {
    output->cell = cell;
}

void output_abstain(struct pipe_t *output) {
    output->cell = NULL;
}
