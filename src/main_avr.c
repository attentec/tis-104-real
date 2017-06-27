#include <stdint.h>

#include "app.h"
#include "disp.h"
#include "font.h"
#include "fonts.h"
#include "indexmap.h"
#include "pin.h"
#include "screen.h"
#include "spi_sw.h"
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

                                                            // Arduino pin
    pin_t led = pin_init(PIN_PORT_D, 2, PIN_DIR_OUTPUT);    // 2
    pin_t clk = pin_init(PIN_PORT_D, 3, PIN_DIR_OUTPUT);    // 3
    pin_t sdi = pin_init(PIN_PORT_D, 4, PIN_DIR_OUTPUT);    // 4
    pin_t rs  = pin_init(PIN_PORT_D, 5, PIN_DIR_OUTPUT);    // 5
    pin_t rst = pin_init(PIN_PORT_D, 6, PIN_DIR_OUTPUT);    // 6
    pin_t cs  = pin_init(PIN_PORT_D, 7, PIN_DIR_OUTPUT);    // 7

    indexmap_init(&indices, WIDTH, HEIGHT, buf);
    screen_init(&scr, &indices);
    spi_init(&spi, sdi, clk);
    disp_init(&disp, &spi, rs, cs, rst, led);
    font_init(&font, monoblipp6x8);
    tft_init(&tft, &disp, &scr, &font);

    app_init(&scr, &tft);
    while (app_loop(&scr, &tft)) {
    }
    app_deinit(&tft);

    return 0;
}
