#pragma once

#include <stdint.h>

struct display_t;
struct font_t;

struct canvas_t {
    struct display_t *display;
};

enum align_t {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

void canvas_init(struct canvas_t *canvas, struct display_t *display);
void canvas_fill_rectangle(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void canvas_clear(struct canvas_t *canvas, uint16_t color);
void canvas_draw_hline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint8_t thickness);
void canvas_draw_vline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t h, uint16_t color, uint8_t thickness);
void canvas_draw_text(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint16_t fg_color, uint16_t bg_color, struct font_t *font, enum align_t align, const char *text);
