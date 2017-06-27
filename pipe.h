#pragma once

#include <stdbool.h>

#include "cpu.h"

struct pipe_t;

bool input_accept(struct pipe_t *input, reg_t *value);

bool output_used(struct pipe_t *output);
bool output_taken(struct pipe_t *output);
void output_offer(struct pipe_t *output, reg_t *cell);
void output_abstain(struct pipe_t *output);
