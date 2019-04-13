#include <SDL2/SDL_surface.h>

#include "panic.h"
#include "surface.h"
#include "surface_sdl.h"

void surface_sdl_init(struct surface_t *surface, struct SDL_Surface *sdl_surface)
{
    surface->surface = sdl_surface;
}

uint8_t surface_get_width(struct surface_t *surface)
{
    return surface->surface->w;
}

uint8_t surface_get_height(struct surface_t *surface)
{
    return surface->surface->h;
}

void surface_set_pixel(struct surface_t *surface, uint8_t x, uint8_t y, uint16_t color)
{
    uint16_t i = ((uint16_t) y * (uint16_t) surface->surface->w) + (uint16_t) x;
    uint16_t *pixels = (uint16_t *) surface->surface->pixels;
    pixels[i] = color;
}
