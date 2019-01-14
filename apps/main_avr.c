#include <stdint.h>

#include "app.h"
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
    struct pin_t led = pin_init(PIN_PORT_D, 2, PIN_DIR_OUTPUT);    // 2
    struct pin_t clk = pin_init(PIN_PORT_D, 3, PIN_DIR_OUTPUT);    // 3
    struct pin_t sdi = pin_init(PIN_PORT_D, 4, PIN_DIR_OUTPUT);    // 4
    struct pin_t rs  = pin_init(PIN_PORT_D, 5, PIN_DIR_OUTPUT);    // 5
    struct pin_t rst = pin_init(PIN_PORT_D, 6, PIN_DIR_OUTPUT);    // 6
    struct pin_t cs  = pin_init(PIN_PORT_D, 7, PIN_DIR_OUTPUT);    // 7
#elif LAYOUT == 2
    // Display module plugged directly into Arduino headers
                                                            // Arduino pin
    struct pin_t led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);    // A0
    //struct pin_t clk = pin_init(PIN_PORT_C, 1, PIN_DIR_OUTPUT);    // A1
    //struct pin_t sdi = pin_init(PIN_PORT_C, 2, PIN_DIR_OUTPUT);    // A2
    struct pin_t rs  = pin_init(PIN_PORT_C, 3, PIN_DIR_OUTPUT);    // A3
    struct pin_t rst = pin_init(PIN_PORT_C, 4, PIN_DIR_OUTPUT);    // A4
    struct pin_t cs  = pin_init(PIN_PORT_C, 5, PIN_DIR_OUTPUT);    // A5
#else
#error Invalid pinout
#endif

    indexmap_init(&indices, WIDTH, HEIGHT, buf);
    screen_init(&scr, &indices);
    spi_init(&spi);
    disp_init(&disp, &spi, &rs, &cs, &rst, &led);
    font_init(&font, monoblipp6x8);
    tft_init(&tft, &disp, &scr, &font);

    app_init(&scr, &tft);
    while (app_loop(&scr, &tft)) {
    }
    app_deinit(&tft);

    return 0;
}
