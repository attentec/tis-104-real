#include <SDL2/SDL_surface.h>

#include "display.h"
#include "display_sdl.h"
#include "panic.h"

#define DEFAULT_WRITE_ORDER (WRITE_ORDER_X_MAJOR)

static void set_current_pixel(struct display_t *display, uint16_t color);
static void advance(struct display_t *display);

void display_sdl_init(struct display_t *display, struct SDL_Surface *surface)
{
    display->surface = surface;
    display->write_order = DEFAULT_WRITE_ORDER;
    display->window_x = 0;
    display->window_y = 0;
    display->window_w = surface->w;
    display->window_h = surface->h;
}

void display_set_write_order(struct display_t *display, enum write_order_t write_order)
{
    display->write_order = write_order;
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
    display->window_x = x;
    display->window_y = y;
    display->window_w = w;
    display->window_h = h;
    display->position_x = 0;
    display->position_y = 0;
}

void display_write_pixel(struct display_t *display, uint16_t color)
{
    set_current_pixel(display, color);
    advance(display);
}

static void set_current_pixel(struct display_t *display, uint16_t color)
{
    uint16_t x = display->window_x + display->position_x;
    uint16_t y = display->window_y + display->position_y;
    uint16_t i = (y * display->surface->w) + x;
    uint16_t *pixels = (uint16_t *) display->surface->pixels;
    pixels[i] = color;
}

static void advance(struct display_t *display)
{
    switch (display->write_order) {
    case WRITE_ORDER_X_MAJOR:
        display->position_x++;
        if (display->position_x < display->window_w) {
            break;
        }
        display->position_x = 0;
        display->position_y++;
        if (display->position_y < display->window_h) {
            break;
        }
        display->position_y = 0;
        break;
    case WRITE_ORDER_Y_MAJOR:
        display->position_y++;
        if (display->position_y < display->window_h) {
            break;
        }
        display->position_y = 0;
        display->position_x++;
        if (display->position_x < display->window_w) {
            break;
        }
        display->position_x = 0;
        break;
    default:
        panic();
        break;
    }
}
