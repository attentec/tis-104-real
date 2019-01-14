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

#define LAYOUT (2)

int main(void) {
    static uint8_t buf[WIDTH * HEIGHT];
    static struct indexmap indices;
    static struct font font;
    static struct screen scr;
    static struct disp_t disp;
    static struct spi_t spi;
    static struct tft_t tft;

#if LAYOUT == 1
    // Display module on prototype shield
                                                            // Arduino pin
    pin_t led = pin_init(PIN_PORT_D, 2, PIN_DIR_OUTPUT);    // 2
    pin_t clk = pin_init(PIN_PORT_D, 3, PIN_DIR_OUTPUT);    // 3
    pin_t sdi = pin_init(PIN_PORT_D, 4, PIN_DIR_OUTPUT);    // 4
    pin_t rs  = pin_init(PIN_PORT_D, 5, PIN_DIR_OUTPUT);    // 5
    pin_t rst = pin_init(PIN_PORT_D, 6, PIN_DIR_OUTPUT);    // 6
    pin_t cs  = pin_init(PIN_PORT_D, 7, PIN_DIR_OUTPUT);    // 7
#elif LAYOUT == 2
    // Display module plugged directly into Arduino headers
                                                            // Arduino pin
    pin_t led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);    // A0
    //pin_t clk = pin_init(PIN_PORT_C, 1, PIN_DIR_OUTPUT);    // A1
    //pin_t sdi = pin_init(PIN_PORT_C, 2, PIN_DIR_OUTPUT);    // A2
    pin_t rs  = pin_init(PIN_PORT_C, 3, PIN_DIR_OUTPUT);    // A3
    pin_t rst = pin_init(PIN_PORT_C, 4, PIN_DIR_OUTPUT);    // A4
    pin_t cs  = pin_init(PIN_PORT_C, 5, PIN_DIR_OUTPUT);    // A5
#else
#error Invalid pinout
#endif

    indexmap_init(&indices, WIDTH, HEIGHT, buf);
    screen_init(&scr, &indices);
    spi_init(&spi);
    disp_init(&disp, &spi, rs, cs, rst, led);
    font_init(&font, monoblipp6x8);
    tft_init(&tft, &disp, &scr, &font);

    tft_begin(&tft);
    tft_set_backlight(&tft, true);

    uint16_t reg = 0x33u;
    uint16_t val0 = 0x0000;
    uint16_t val1 = 0x0000;

    for (;;) {
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            if ((y >> 4) & 1) {
                disp_write_register(&disp, reg, val0);
            } else {
                disp_write_register(&disp, reg, val1);
            }
            disp_write_command(&disp, 0x22u);
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
                disp_write_data(&disp, RGB888_TO_RGB565(color));
            }
        }
        for (uint8_t y = 0; y < TFT_HEIGHT; y++) {
            if ((y >> 4) & 1) {
                disp_write_register(&disp, reg, val0);
            } else {
                disp_write_register(&disp, reg, val1);
            }
            disp_write_command(&disp, 0x22u);
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
                disp_write_data(&disp, RGB888_TO_RGB565(color));
            }
        }
    }
    return 0;
}
