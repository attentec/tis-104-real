#pragma once

#include "display.h"

struct SDL_Surface;

struct display_t {
    struct SDL_Surface *surface;
};

void display_sdl_init(struct display_t *display, struct SDL_Surface *surface);
