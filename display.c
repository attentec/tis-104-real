// Include application, user and local libraries
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include <font.h>
#include <indexmap.h>
#include <screen.h>

#include "tft.h"

#define TFT_RST 2
#define TFT_RS  3
#define TFT_CS  4  // SS
#define TFT_SDI 5  // MOSI
#define TFT_CLK 6  // SCK
#define TFT_LED 7   // 0 if wired to +5V directly

static void set_pin_high(void) {
    PORTB |= (1 << 0);
}

static void set_pin_low(void) {
    PORTB &= ~(1 << 0);
}

int main() {
    indexmap indices = indexmap_init(20, 20);
    FontInfo font = font_make_fontinfo(Terminal6x8);
    screen scr = screen_init(indices, &font);

    // Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
    //TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
    // Use software SPI (slower)
    tft_swspi(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED, scr);

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
