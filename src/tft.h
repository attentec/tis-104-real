#pragma once

#include <stdbool.h>
#include <stdint.h>

struct dispif_t;
struct font;
struct screen;

#define TFT_WIDTH  (176u)
#define TFT_HEIGHT (220u)

#define RGB888_TO_RGB565(x) ((((x >> 19u) & 0x1Fu) << 11u) | \
                             (((x >> 10u) & 0x3Fu) <<  5u) | \
                             (((x >>  3u) & 0x1Fu) <<  0u))

#define COLOR_BLACK    RGB888_TO_RGB565(0x000000uL)
#define COLOR_WHITE    RGB888_TO_RGB565(0xFFFFFFuL)
#define COLOR_DARKGREY RGB888_TO_RGB565(0xA9A9A9uL)

struct tft_t {
    uint16_t max_x;
    uint16_t max_y;
    uint16_t bg_color;
    uint16_t fg_color;

    struct font *font;
    struct dispif_t *dispif;
    struct screen *scr;
};

void tft_init(struct tft_t *tft, struct dispif_t *dispif, struct screen *scr, struct font *font);
void tft_begin(struct tft_t *tft);
void tft_clear(struct tft_t *tft);
void tft_set_backlight(struct tft_t *tft, bool flag);
void tft_set_background_color(struct tft_t *tft, uint16_t color);
void tft_set_foreground_color(struct tft_t *tft, uint16_t color);
void tft_fill_rectangle(struct tft_t *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void tft_draw_char(struct tft_t *tft, uint8_t x, uint8_t y, char ch);
