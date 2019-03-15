#include <stdint.h>

#include "board.h"
#include "display.h"

int main(void) {
    struct display_t display;
    board_init();
    display_init(&display, board.dispif, ORIENTATION_RIBBON_BOTTOM);
    display_set_write_order(&display, WRITE_ORDER_X_MAJOR);
    display_activate(&display);

    uint8_t w = display_get_width(&display);
    uint8_t h = display_get_height(&display);
    display_set_window(&display, 0, 0, w, h);
    for (;;) {
        for (uint8_t y = 0; y < h; y++) {
            for (uint8_t x = 0; x < w; x++) {
                uint8_t red = ((uint16_t) x * 255u) / 175u;
                uint8_t green = ((uint16_t) y * 255u) / 219u;
                uint8_t blue = 0;
                uint32_t color = 0;
                color |= red;
                color <<= 8u;
                color |= green;
                color <<= 8u;
                color |= blue;
                display_write_pixel(&display, RGB888_TO_RGB565(color));
            }
        }
        for (uint8_t y = 0; y < h; y++) {
            for (uint8_t x = 0; x < w; x++) {
                uint8_t red = ((uint16_t) y * 255u) / 219u;
                uint8_t green = 255;
                uint8_t blue = ((uint16_t) x * 255u) / 175u;
                uint32_t color = 0;
                color |= red;
                color <<= 8u;
                color |= green;
                color <<= 8u;
                color |= blue;
                display_write_pixel(&display, RGB888_TO_RGB565(color));
            }
        }
    }
    return 0;
}
