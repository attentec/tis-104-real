// Include application, user and local libraries
#include <stdio.h>
#include <stdlib.h>

#include "font.h"
#include "fonts.h"
#include "indexmap.h"
#include "screen.h"
#include "tft.h"
#include "delay.h"

#define WIDTH (29)
#define HEIGHT (27)

const uint8_t middle = 20;
const uint8_t right = 27;
const uint8_t bottom = 15;


void draw_background(struct screen *scr) {
    tft_setForegroundColor(COLOR_WHITE);

    for (uint8_t y = 1; y < bottom; ++y) {
        screen_set(scr, 0, y, 0x01);
        screen_set(scr, middle, y, 0x01);
        screen_set(scr, right, y, 0x01);
    }
    for (uint8_t x = 1; x < right; ++x) {
        screen_set(scr, x, 0, 0x00);
        screen_set(scr, x, bottom, 0x00);
    }
    for (uint8_t y = 3; y < bottom; y += 3) {
        for (uint8_t x = middle; x < right; ++x) {
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

    tft_drawText(middle + 2, 1, "ACC");
    tft_drawText(middle + 2, 4, "BAK");
    tft_drawText(middle + 2, 7, "LAST");
    tft_drawText(middle + 2, 10, "MODE");
    tft_drawText(middle + 2, 13, "IDLE");


    tft_render();


    tft_setForegroundColor(COLOR_WHITE);
}

void draw_acc(int16_t acc) {
    char buffer[5];
    sprintf(buffer, "%4d", acc);
    buffer[4] = '\0';
    tft_drawText(middle + 2, 2, buffer);
}

void draw_bak(int16_t bak) {
    char buffer[7];
    sprintf(buffer, "<%4d>", bak);
    buffer[6] = '\0';
    tft_drawText(middle + 1, 5, buffer);
}

void draw_last(char *last) {
    tft_drawText(middle + 2, 8, last);
}

void draw_mode(char *mode) {
    tft_drawText(middle + 2, 11, mode);
}

void draw_idle(uint8_t idle) {
    char buffer[5];
    sprintf(buffer, "%3d%%", idle);
    buffer[4] = '\0';
    tft_drawText(middle + 2, 14, buffer);
}

int main(void) {
    static uint8_t buf[WIDTH * HEIGHT];
    static struct indexmap indices;
    static struct font font;
    static struct screen scr;

    indexmap_init(&indices, WIDTH, HEIGHT, buf);
    font_init(&font, Terminal6x8);
    screen_init(&scr, &indices, &font);

    // Set up pins
    pin_t led = pin_init(PIN_PORT_D, 2, PIN_DIR_OUTPUT);    // 2
    pin_t clk = pin_init(PIN_PORT_D, 3, PIN_DIR_OUTPUT);    // 3
    pin_t sdi = pin_init(PIN_PORT_D, 4, PIN_DIR_OUTPUT);    // 4
    pin_t rs = pin_init(PIN_PORT_D, 5, PIN_DIR_OUTPUT);     // 5
    pin_t rst = pin_init(PIN_PORT_D, 6, PIN_DIR_OUTPUT);    // 6
    pin_t cs = pin_init(PIN_PORT_D, 7, PIN_DIR_OUTPUT);     // 7

    // Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
    //TFT_22_ILI9225 tft = TFT_22_ILI9225(rst, rs, cs, led);
    // Use software SPI (slower)
    tft_swspi(rst, rs, cs, sdi, clk, led, &scr);

    tft_setFont(&font);

    tft_begin();
    draw_background(&scr);
    tft_render();

    static int16_t acc = 0;
    static int16_t bak = 0;
    while (true) {
        if (acc >= 999) {
            acc = -999;
        }
        if (bak >= 999) {
            bak = -999;
        }
        draw_acc(acc);
        draw_bak(bak);
        draw_last("N/A");
        draw_mode("IDLE");
        draw_idle(0);
        tft_render();
        acc += 13;
        bak += 29;
    }

    tft_setBacklight(false);
    tft_setDisplay(false);

    return 0;
}
