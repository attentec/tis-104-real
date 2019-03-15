#include <SDL2/SDL_surface.h>

#include "display.h"
#include "display_sdl.h"

void display_sdl_init(struct display_t *display, struct SDL_Surface *surface)
{
    display->surface = surface;
}

void display_set_write_order(struct display_t *display, enum write_order_t write_order)
{
    (void) display;
    (void) write_order;
}

void display_activate(struct display_t *display)
{
    (void) display;
}

uint8_t display_get_width(struct display_t *display)
{
    return display->surface->w;
}

uint8_t display_get_height(struct display_t *display)
{
    return display->surface->h;
}

void display_set_window(struct display_t *display, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    (void) display;
    (void) x;
    (void) y;
    (void) w;
    (void) h;
}

void display_write_pixel(struct display_t *display, uint16_t color)
{
    (void) display;
    (void) color;
}
