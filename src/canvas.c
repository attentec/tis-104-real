#include "canvas.h"
#include "display.h"

void canvas_init(struct canvas_t *canvas, struct display_t *display)
{
    canvas->display = display;
}

void canvas_fill_rectangle(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
    display_set_window(canvas->display, x, y, w, h);
    for (uint16_t i = 0; i < (uint16_t) w * (uint16_t) h; i++) {
        display_write_pixel(canvas->display, color);
    }
}

void canvas_clear(struct canvas_t *canvas, uint16_t color)
{
    uint8_t w = display_get_width(canvas->display);
    uint8_t h = display_get_height(canvas->display);
    canvas_fill_rectangle(canvas, 0, 0, w, h, color);
}

void canvas_draw_hline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint8_t thickness)
{
    uint8_t t = thickness / 2;
    canvas_fill_rectangle(canvas, x - t, y - t, w + (2 * t) + 1, (2 * t) + 1, color);
}

void canvas_draw_vline(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t h, uint16_t color, uint8_t thickness)
{
    uint8_t t = thickness / 2;
    canvas_fill_rectangle(canvas, x - t, y - t, (2 * t) + 1, h + (2 * t) + 1, color);
}
