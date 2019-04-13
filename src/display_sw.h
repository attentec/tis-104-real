#pragma once

#include <stdint.h>

#include "display.h"

struct surface_t;

struct display_t {
    struct surface_t *surface;
    enum write_order_t write_order;
    // Window rectangle
    uint8_t window_x;
    uint8_t window_y;
    uint8_t window_w;
    uint8_t window_h;
    // Position relative to window top left
    uint8_t position_x;
    uint8_t position_y;
};

void display_sw_init(struct display_t *display, struct surface_t *surface);
