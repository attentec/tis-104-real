#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "screen.h"
#include "tft.h"

const uint8_t middle = 20;
const uint8_t right = 27;
const uint8_t bottom = 15;

static void render(struct screen *scr, struct tft_t *tft) {
    DirtyIterator dirties;
    screen_get_dirties(scr, &dirties);
    while (screen_get_next_dirty(&dirties)) {
        tft_draw_char(tft, dirties.x, dirties.y, screen_get(scr, dirties.x, dirties.y));
    }
}

static void draw_text(struct screen *scr, uint8_t x, uint8_t y, char *s) {
    uint8_t currx = x;

    // Print every character in string
    for (uint8_t k = 0; k < strlen(s); k++) {
        screen_set(scr, currx, y, s[k]);
        currx += 1;
    }
}

static void draw_background(struct screen *scr, struct tft_t *tft) {
    tft_set_foreground_color(tft, COLOR_WHITE);

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


    render(scr, tft);


    tft_set_foreground_color(tft, COLOR_DARKGREY);

    draw_text(scr, middle + 2, 1, "ACC");
    draw_text(scr, middle + 2, 4, "BAK");
    draw_text(scr, middle + 2, 7, "LAST");
    draw_text(scr, middle + 2, 10, "MODE");
    draw_text(scr, middle + 2, 13, "IDLE");


    render(scr, tft);


    tft_set_foreground_color(tft, COLOR_WHITE);
}

static void draw_acc(struct screen *scr, int16_t acc) {
    char buffer[5];
    sprintf(buffer, "%4d", acc);
    buffer[4] = '\0';
    draw_text(scr, middle + 2, 2, buffer);
}

static void draw_bak(struct screen *scr, int16_t bak) {
    char buffer[7];
    sprintf(buffer, "<%4d>", bak);
    buffer[6] = '\0';
    draw_text(scr, middle + 1, 5, buffer);
}

static void draw_last(struct screen *scr, char *last) {
    draw_text(scr, middle + 2, 8, last);
}

static void draw_mode(struct screen *scr, char *mode) {
    draw_text(scr, middle + 2, 11, mode);
}

static void draw_idle(struct screen *scr, uint8_t idle) {
    char buffer[5];
    sprintf(buffer, "%3d%%", idle);
    buffer[4] = '\0';
    draw_text(scr, middle + 2, 14, buffer);
}


static void gui_init(struct screen *scr, struct tft_t *tft) {
    tft_begin(tft);
    tft_set_background_color(tft, COLOR_BLACK);
    tft_set_foreground_color(tft, COLOR_WHITE);
    tft_clear(tft);
    draw_background(scr, tft);
    tft_set_backlight(tft, true);
    render(scr, tft);
}

static bool gui_loop(struct screen *scr, struct tft_t *tft) {
    static int16_t acc = 0;
    static int16_t bak = 0;

    if (acc >= 999) {
        acc = -999;
    }
    if (bak >= 999) {
        bak = -999;
    }
    draw_acc(scr, acc);
    draw_bak(scr, bak);
    draw_last(scr, "N/A");
    draw_mode(scr, "IDLE");
    draw_idle(scr, 0);
    render(scr, tft);
    acc += 13;
    bak += 29;

    return true;
}

static void gui_deinit(struct tft_t *tft) {
    tft_set_backlight(tft, false);
}

int main(void) {
    struct board_t board;
    board_init(&board);

    gui_init(board.scr, board.tft);
    while (gui_loop(board.scr, board.tft)) {
    }
    gui_deinit(board.tft);

    return 0;
}
