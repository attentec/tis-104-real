#pragma once

#include <stdint.h>

struct display_t;
struct font_t;
struct icon_t;

struct canvas_t {
    struct display_t *display;
    struct font_t *font;
    uint16_t fg_color;
    uint16_t bg_color;
    uint8_t thickness;
};

enum align_t {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};

enum rot_t {
    ROT_0,
    ROT_90,
    ROT_180,
    ROT_270
};

void canvas_init(struct canvas_t *canvas, struct display_t *display, struct font_t *font);
void canvas_set_font(struct canvas_t *canvas, struct font_t *font);
void canvas_set_fg_color(struct canvas_t *canvas, uint16_t fg_color);
void canvas_set_bg_color(struct canvas_t *canvas, uint16_t bg_color);
void canvas_set_thickness(struct canvas_t *canvas, uint8_t thickness);
void canvas_fill_rectangle(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void canvas_clear(struct canvas_t *canvas, uint16_t color);
void canvas_draw_hline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w);
void canvas_draw_vline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t h);
void canvas_draw_text(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, enum align_t align, const char *text);
void canvas_draw_icon(struct canvas_t *canvas, uint8_t x, uint8_t y, enum rot_t rot, struct icon_t *icon);
