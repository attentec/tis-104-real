#pragma once

#include <stdint.h>

struct display_t;

struct canvas_t {
    struct display_t *display;
};

void canvas_init(struct canvas_t *canvas, struct display_t *display);
void canvas_fill_rectangle(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void canvas_clear(struct canvas_t *canvas, uint16_t color);
void canvas_draw_hline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint8_t thickness);
void canvas_draw_vline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t h, uint16_t color, uint8_t thickness);
