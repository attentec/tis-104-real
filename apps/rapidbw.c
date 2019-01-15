#include <stdint.h>

#include "board.h"
#include "dispif.h"
#include "tft.h"

int main(void) {
    struct board_t board;
    board_init(&board);

    tft_begin(board.tft);
    tft_set_backlight(board.tft, true);

    for (;;) {
        dispif_write_command(board.dispif, 0x22u);
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                dispif_write_data(board.dispif, COLOR_WHITE);
            }
        }
        dispif_write_command(board.dispif, 0x22u);
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                dispif_write_data(board.dispif, COLOR_BLACK);
            }
        }
    }
    return 0;
}
