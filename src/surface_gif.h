#pragma once

#include "surface.h"

struct surface_t {
    GifPixelType *pixels;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
};

void surface_gif_init(struct surface_t *surface, GifPixelType *pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
