#include <stdint.h>

#include "board.h"
#include "disp.h"
#include "tft.h"

int main(void) {
    struct board_t board;
    board_init(&board);

    tft_begin(board.tft);
    tft_set_backlight(board.tft, true);

    for (;;) {
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            disp_write_command(board.disp, 0x22u);
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                uint8_t red = ((uint16_t) x * 255u) / 175u;
                uint8_t green = ((uint16_t) y * 255u) / 219u;
                uint8_t blue = 0;
                uint32_t color = 0;
                color |= red;
                color <<= 8u;
                color |= green;
                color <<= 8u;
                color |= blue;
                disp_write_data(board.disp, RGB888_TO_RGB565(color));
            }
        }
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            disp_write_command(board.disp, 0x22u);
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                uint8_t red = ((uint16_t) y * 255u) / 219u;
                uint8_t green = 255;
                uint8_t blue = ((uint16_t) x * 255u) / 175u;
                uint32_t color = 0;
                color |= red;
                color <<= 8u;
                color |= green;
                color <<= 8u;
                color |= blue;
                disp_write_data(board.disp, RGB888_TO_RGB565(color));
            }
        }
    }
    return 0;
}
