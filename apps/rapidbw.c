#include <stdint.h>

#include "board.h"
#include "display.h"

int main(void) {
    struct display_t display;
    board_init();
    display_init(&display, board.dispif, ORIENTATION_RIBBON_BOTTOM);
    display_set_write_order(&display, WRITE_ORDER_X_MAJOR);
    display_activate(&display);

    uint16_t white = RGB888_TO_RGB565(0xFFFFFFul);
    uint16_t black = RGB888_TO_RGB565(0x000000ul);
    uint8_t w = display_get_width(&display);
    uint8_t h = display_get_height(&display);
    display_set_window(&display, 0, 0, w, h);
    for (;;) {
        for (uint8_t y = 0; y < h; y++) {
            for (uint8_t x = 0; x < w; x++) {
                display_write_pixel(&display, white);
            }
        }
        for (uint8_t y = 0; y < h; y++) {
            for (uint8_t x = 0; x < w; x++) {
                display_write_pixel(&display, black);
            }
        }
    }
    return 0;
}
