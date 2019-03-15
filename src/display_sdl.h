#pragma once

#include <stdint.h>

#include "display.h"

struct SDL_Surface;

struct display_t {
    struct SDL_Surface *surface;
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

void display_sdl_init(struct display_t *display, struct SDL_Surface *surface);
