#include <stdint.h>

#include "board.h"
#include "display.h"
#include "dispif.h"

int main(void)
{
    struct board_t board;
    struct display_t display;

    board_init(&board);
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_X_MAJOR);
    display_clear(&display, 0xFFFF);
    display_set_window(&display, 24, 0, 128, 128);
    for (uint8_t y = 0; y < 128; y++) {
        for (uint8_t x = 0; x < 128; x++) {
            uint8_t red = (uint16_t) x * 2;
            uint8_t green = (uint16_t) y * 2;
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
    display_activate(&display);

    for (;;) {
    }

    return 0;
}
