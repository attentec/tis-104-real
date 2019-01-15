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
        disp_write_command(board.disp, 0x22u);
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                disp_write_data(board.disp, COLOR_WHITE);
            }
        }
        disp_write_command(board.disp, 0x22u);
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                disp_write_data(board.disp, COLOR_BLACK);
            }
        }
    }
    return 0;
}
