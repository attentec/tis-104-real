#pragma once

#include "surface.h"

struct SDL_Surface;

struct surface_t {
    struct SDL_Surface *surface;
};

void surface_sdl_init(struct surface_t *surface, struct SDL_Surface *sdl_surface);
