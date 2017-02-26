#include "pipe.h"
#include "pipe_mock.h"

void input_request(struct pipe_t *input) {
    input->reader_waiting = true;
}

bool input_accept(struct pipe_t *input, reg_t *value) {
    if (input->value_available) {
        input->reader_waiting = false;
        input->value_available = false;
        *value = input->value;
        return true;
    } else {
        return false;
    }
}

void input_decline(struct pipe_t *input) {
    (void)input;
}

bool output_offer(struct pipe_t *output, reg_t value) {
    if (output->reader_waiting) {
        output->value = value;
        output->value_available = true;
        return true;
    } else {
        return false;
    }
}

void output_abstain(struct pipe_t *output) {
    (void)output;
}
