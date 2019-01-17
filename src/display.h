#pragma once

#define RGB888_TO_RGB565(x) ((((x >> 19u) & 0x1Fu) << 11u) | \
                             (((x >> 10u) & 0x3Fu) <<  5u) | \
                             (((x >>  3u) & 0x1Fu) <<  0u))

struct dispif_t;

enum orientation_t {
    ORIENTATION_RIBBON_BOTTOM,
    ORIENTATION_RIBBON_LEFT,
    ORIENTATION_RIBBON_TOP,
    ORIENTATION_RIBBON_RIGHT
};

enum write_order_t {
    WRITE_ORDER_X_MAJOR,
    WRITE_ORDER_Y_MAJOR
};

struct display_t {
    struct dispif_t *dispif;
    enum orientation_t orientation;
};

void display_init(struct display_t *display, struct dispif_t *dispif, enum orientation_t orientation, enum write_order_t write_order);
void display_activate(struct display_t *display);
uint8_t display_get_width(struct display_t *display);
uint8_t display_get_height(struct display_t *display);
void display_set_window(struct display_t *display, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void display_write_pixel(struct display_t *display, uint16_t color);
void display_fill_rectangle(struct display_t *display, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void display_clear(struct display_t *display, uint16_t color);
