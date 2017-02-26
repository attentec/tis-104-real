#pragma once

#include <stdbool.h>
#include "cpu.h"

struct pipe_t;

void input_request(struct pipe_t *input);
bool input_accept(struct pipe_t *input, reg_t *value);
void input_decline(struct pipe_t *input);

bool output_offer(struct pipe_t *output, reg_t value);
void output_abstain(struct pipe_t *output);
