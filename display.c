// Include application, user and local libraries
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include <font.h>
#include <indexmap.h>
#include <screen.h>

#include "tft.h"

static void set_pin_high(void) {
    PORTB |= (1 << 0);
}

static void set_pin_low(void) {
    PORTB &= ~(1 << 0);
}

int main() {
    struct indexmap indices;
    uint8_t buf[20 * 20];
    indexmap_init(&indices, 20, 20, buf);

    struct font font;
    font_init(&font, Terminal6x8);

    screen scr = screen_init(&indices, &font);

    // Set up pins
    pin_t rs = pin_init(PIN_PORT_D, 3, PIN_DIR_OUTPUT);
    pin_t cs = pin_init(PIN_PORT_D, 4, PIN_DIR_OUTPUT);
    pin_t rst = pin_init(PIN_PORT_D, 2, PIN_DIR_OUTPUT);
    pin_t led = pin_init(PIN_PORT_D, 7, PIN_DIR_OUTPUT);
    pin_t clk = pin_init(PIN_PORT_D, 6, PIN_DIR_OUTPUT);
    pin_t sdi = pin_init(PIN_PORT_D, 5, PIN_DIR_OUTPUT);

    // Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
    //TFT_22_ILI9225 tft = TFT_22_ILI9225(rst, rs, cs, led);
    // Use software SPI (slower)
    tft_swspi(rst, rs, cs, sdi, clk, led, scr);

    tft_setFont(&font);

    tft_begin();

    screen_set(scr, 0, 0, 0x63);
    screen_set(scr, 19, 0, 0x62);
    screen_set(scr, 0, 19, 0x65);
    screen_set(scr, 19, 19, 0x64);
    for (int i = 1; i < 19; ++i) {
        screen_set(scr, i, 0, 0x60);
        screen_set(scr, i, 19, 0x60);
        screen_set(scr, 0, i, 0x61);
        screen_set(scr, 19, i, 0x61);
    }
    tft_render();

    DDRB |= (1 << 0);
    DDRB |= (1 << 1);


    int count = 0;
    char buffer[5];
    while (true) {
        sprintf(buffer, "%04d", count);
        buffer[4] = '\0';
        tft_drawText(10, 10, buffer, COLOR_WHITE);
        set_pin_high();
        tft_render();
        set_pin_low();
        count += 1;
    }

    tft_setBacklight(false);
    tft_setDisplay(false);

    return 0;
}
