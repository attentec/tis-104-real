#pragma once

#include <stdint.h>

#include "code.h"

struct canvas_t;
struct state_t;

void gui_show_cpu(struct canvas_t *canvas, const source_t *lines);
void gui_update_cpu(struct canvas_t *canvas, const source_t *lines, struct state_t *cpu_state, uint8_t last_line, uint8_t current_line);
