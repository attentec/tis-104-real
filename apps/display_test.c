#include <stdint.h>

#include "disp.h"
#include "font.h"
#include "fonts.h"
#include "indexmap.h"
#include "pin.h"
#include "screen.h"
#include "spi_hw.h"
#include "tft.h"

#define WIDTH (29)
#define HEIGHT (27)

int main(void) {
    static uint8_t buf[WIDTH * HEIGHT];
    static struct indexmap indices;
    static struct font font;
    static struct screen scr;
    static struct disp_t disp;
    static struct spi_t spi;
    static struct tft_t tft;

    // Display module plugged directly into Arduino headers
                                                            // Arduino pin
    struct pin_t led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);    // A0
    //struct pin_t clk = pin_init(PIN_PORT_C, 1, PIN_DIR_OUTPUT);    // A1
    //struct pin_t sdi = pin_init(PIN_PORT_C, 2, PIN_DIR_OUTPUT);    // A2
    struct pin_t rs  = pin_init(PIN_PORT_C, 3, PIN_DIR_OUTPUT);    // A3
    struct pin_t rst = pin_init(PIN_PORT_C, 4, PIN_DIR_OUTPUT);    // A4
    struct pin_t cs  = pin_init(PIN_PORT_C, 5, PIN_DIR_OUTPUT);    // A5

    indexmap_init(&indices, WIDTH, HEIGHT, buf);
    screen_init(&scr, &indices);
    spi_init(&spi);
    disp_init(&disp, &spi, &rs, &cs, &rst, &led);
    font_init(&font, monoblipp6x8);
    tft_init(&tft, &disp, &scr, &font);

    tft_begin(&tft);
    tft_set_backlight(&tft, true);

    for (;;) {
        disp_write_command(&disp, 0x22u);
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                disp_write_data(&disp, COLOR_WHITE);
            }
        }
        disp_write_command(&disp, 0x22u);
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            for (uint8_t x = 0; x < TFT_WIDTH; x++) {
                disp_write_data(&disp, COLOR_BLACK);
            }
        }
    }
    return 0;
}
