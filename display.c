// Include application, user and local libraries
#include <stdio.h>
#include <stdlib.h>

#include "font.h"
#include "indexmap.h"
#include "screen.h"
#include "tft.h"
#include "delay.h"

#define WIDTH (29)
#define HEIGHT (27)

void draw_background(screen scr) {
    const uint8_t left = 0;
    const uint8_t middle = 20;
    const uint8_t right = 25;
    const uint8_t bottom = 15;


    tft_setForegroundColor(COLOR_WHITE);

    for (int y = 1; y < bottom; ++y) {
        screen_set(scr, 0, y, 0x01);
        screen_set(scr, middle, y, 0x01);
        screen_set(scr, right, y, 0x01);
    }
    for (int x = 1; x < right; ++x) {
        screen_set(scr, x, 0, 0x00);
        screen_set(scr, x, bottom, 0x00);
    }
    for (int y = 3; y < bottom; y += 3) {
        for (int x = middle; x < right; ++x) {
            screen_set(scr, x, y, 0x00);
        }
        screen_set(scr, middle, y, 0x07);
        screen_set(scr, right, y, 0x06);
    }
    screen_set(scr, 0, 0, 0x03);
    screen_set(scr, middle, 0, 0x09);
    screen_set(scr, right, 0, 0x02);
    screen_set(scr, 0, bottom, 0x05);
    screen_set(scr, middle, bottom, 0x08);
    screen_set(scr, right, bottom, 0x04);


    tft_render();


    tft_setForegroundColor(COLOR_DARKGRAY);

    tft_drawText(middle + 1, 1, "ACC");
    tft_drawText(middle + 1, 4, "BAK");
    tft_drawText(middle + 1, 7, "LAST");
    tft_drawText(middle + 1, 10, "MODE");
    tft_drawText(middle + 1, 13, "IDLE");


    tft_render();


    tft_setForegroundColor(COLOR_WHITE);
}

int main(void) {
    struct indexmap indices;
    uint8_t buf[WIDTH * HEIGHT];
    indexmap_init(&indices, WIDTH, HEIGHT, buf);

    struct font font;
    font_init(&font, Terminal6x8);

    screen scr = screen_init(&indices, &font);

    // Set up pins
    pin_t cs = pin_init(PIN_PORT_D, 2, PIN_DIR_OUTPUT);     //  2
    pin_t rst = pin_init(PIN_PORT_D, 4, PIN_DIR_OUTPUT);    //  4
    pin_t rs = pin_init(PIN_PORT_D, 6, PIN_DIR_OUTPUT);     //  6
    pin_t sdi = pin_init(PIN_PORT_B, 0, PIN_DIR_OUTPUT);    //  8
    pin_t clk = pin_init(PIN_PORT_B, 2, PIN_DIR_OUTPUT);    // 10
    pin_t led = pin_init(PIN_PORT_B, 4, PIN_DIR_OUTPUT);    // 12

    // Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
    //TFT_22_ILI9225 tft = TFT_22_ILI9225(rst, rs, cs, led);
    // Use software SPI (slower)
    tft_swspi(rst, rs, cs, sdi, clk, led, scr);

    tft_setFont(&font);

    tft_begin();
    draw_background(scr);
    tft_render();

    int count = 0x00;
    char buffer[5];
    while (true) {
        if (count >= 0x80) {
            count = 0x00;
        }
        sprintf(buffer, "%03d", count);
        buffer[3] = count;
        buffer[4] = '\0';
        tft_drawText(10, 10, buffer);
        tft_render();
        count += 1;
        delay_ms(500);
    }

    tft_setBacklight(false);
    tft_setDisplay(false);

    return 0;
}
