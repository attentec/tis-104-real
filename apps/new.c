#include <stdint.h>

#include "board.h"
#include "display.h"
#include "dispif.h"

int main(void)
{
    struct board_t board;
    struct display_t display;

    board_init(&board);
    display_init(&display, board.dispif);
    display_set_window(&display, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++) {
        for (uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
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
    display_activate(&display);

    for (;;) {
    }

    return 0;
}
