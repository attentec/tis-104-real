#pragma once

#define RGB888_TO_RGB565(x) ((((x >> 19u) & 0x1Fu) << 11u) | \
                             (((x >> 10u) & 0x3Fu) <<  5u) | \
                             (((x >>  3u) & 0x1Fu) <<  0u))

enum write_order_t {
    WRITE_ORDER_X_MAJOR,
    WRITE_ORDER_Y_MAJOR
};

struct display_t;

void display_set_write_order(struct display_t *display, enum write_order_t write_order);
void display_activate(struct display_t *display);
uint8_t display_get_width(struct display_t *display);
uint8_t display_get_height(struct display_t *display);
void display_set_window(struct display_t *display, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void display_write_pixel(struct display_t *display, uint16_t color);
