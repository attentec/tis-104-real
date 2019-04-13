#include "display.h"
#include "display_sw.h"
#include "panic.h"
#include "surface.h"

#define DEFAULT_WRITE_ORDER (WRITE_ORDER_X_MAJOR)

static void set_current_pixel(struct display_t *display, uint16_t color);
static void advance(struct display_t *display);

void display_sw_init(struct display_t *display, struct surface_t *surface)
{
    display->surface = surface;
    display->write_order = DEFAULT_WRITE_ORDER;
    display->window_x = 0;
    display->window_y = 0;
    display->window_w = surface_get_width(surface);
    display->window_h = surface_get_height(surface);
}

void display_set_write_order(struct display_t *display, enum write_order_t write_order)
{
    display->write_order = write_order;
}

uint8_t display_get_width(struct display_t *display)
{
    return surface_get_width(display->surface);
}

uint8_t display_get_height(struct display_t *display)
{
    return surface_get_height(display->surface);
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
    uint8_t x = display->window_x + display->position_x;
    uint8_t y = display->window_y + display->position_y;
    surface_set_pixel(display->surface, x, y, color);
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
