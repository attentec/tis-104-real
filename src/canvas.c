#include "canvas.h"
#include "display.h"
#include "font.h"

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

void canvas_draw_text(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint16_t fg_color, uint16_t bg_color, struct font_t *font, const char *text)
{
    display_set_window(canvas->display, x, y, w, font->height);
    uint8_t text_width = 0;
    for (const char *char_ptr = text; *char_ptr != '\0'; char_ptr++) {
        for (uint8_t col = 0; col < font->width; col++) {
            uint8_t col_pixels = font_read_column(font, (uint8_t) *char_ptr, col);
            for (uint8_t row = 0; row < font->height; row++) {
                uint16_t color = col_pixels & 0x01 ? fg_color : bg_color;
                col_pixels >>= 1;
                display_write_pixel(canvas->display, color);
            }
            text_width++;
        }
    }
    for (uint8_t col = text_width; col < w; col++) {
        for (uint8_t row = 0; row < font->height; row++) {
            display_write_pixel(canvas->display, bg_color);
        }
    }
}
