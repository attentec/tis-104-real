#pragma once

#include "cpu.h"

struct canvas_t;
struct code_t;

struct tile_t {
    struct code_t code;
    struct canvas_t *canvas;
    struct state_t cpu_state;
    struct cpu_t cpu;
    address_t last_line;
};

void tile_init(struct tile_t *tile, struct code_t *code, struct canvas_t *canvas);
void tile_draw(struct tile_t *tile);
void tile_read(struct tile_t *tile);
void tile_write(struct tile_t *tile);
