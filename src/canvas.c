#include "canvas.h"
#include "display.h"
#include "font.h"
#include "panic.h"

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

void canvas_draw_text(struct canvas_t *canvas, uint8_t x, uint8_t y, uint8_t w, uint16_t fg_color, uint16_t bg_color, struct font_t *font, enum align_t align, const char *text)
{
    display_set_window(canvas->display, x, y, w, font->height);
    uint8_t text_width = 0;
    for (const char *char_ptr = text; *char_ptr != '\0'; char_ptr++) {
        text_width += font->width;
    }
    uint8_t left_paddding = 0;
    if (text_width < w) {
        switch (align) {
        case ALIGN_LEFT:
            left_paddding = 0;
            break;
        case ALIGN_CENTER:
            left_paddding = (w - text_width) / 2;
            break;
        case ALIGN_RIGHT:
            left_paddding = w - text_width;
            break;
        default:
            panic();
            break;
        }
    }
    uint8_t current_width = 0;
    for (; current_width < left_paddding; current_width++) {
        for (uint8_t row = 0; row < font->height; row++) {
            display_write_pixel(canvas->display, bg_color);
        }
    }
    for (const char *char_ptr = text; *char_ptr != '\0'; char_ptr++) {
        for (uint8_t col = 0; col < font->width && current_width < w; col++, current_width++) {
            uint8_t col_pixels = font_read_column(font, (uint8_t) *char_ptr, col);
            for (uint8_t row = 0; row < font->height; row++) {
                uint16_t color = col_pixels & 0x01 ? fg_color : bg_color;
                col_pixels >>= 1;
                display_write_pixel(canvas->display, color);
            }
        }
    }
    for (; current_width < w; current_width++) {
        for (uint8_t row = 0; row < font->height; row++) {
            display_write_pixel(canvas->display, bg_color);
        }
    }
}
