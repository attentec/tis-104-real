#include <stdint.h>

#include "board.h"
#include "display.h"
#include "dispif.h"

static void draw_static(struct display_t *display);

int main(void)
{
    struct board_t board;
    struct display_t display;

    board_init(&board);
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_X_MAJOR);
    draw_static(&display);
    display_activate(&display);

    for (;;) {
    }

    return 0;
}

static uint8_t char_width  = 6;
static uint8_t char_height = 8;

static uint8_t main_x_pixels       = 26;
static uint8_t main_y_pixels       = 20;
static uint8_t code_width_chars    = 19;
static uint8_t code_height_chars   = 15;
static uint8_t status_width_chars  =  6;
static uint8_t status_height_chars =  2;

static uint16_t white = RGB888_TO_RGB565(0xFFFFFFul);
static uint16_t black = RGB888_TO_RGB565(0x000000ul);

static void draw_static(struct display_t *display)
{
    display_clear(display, black);

    uint8_t x0 = main_x_pixels + 3;
    uint8_t w1 = char_width * (code_width_chars + 1);
    uint8_t w2 = char_width * (status_width_chars + 1);
    uint8_t y0 = main_y_pixels + 3;
    uint8_t h  = char_height * (code_height_chars + 1);
    uint8_t hs = char_height * (status_height_chars + 1);

    display_draw_hline(display, x0,    y0,          w1+w2, white, 3);
    display_draw_hline(display, x0+w1, y0+(1*hs)+1, w2,    white, 3);
    display_draw_hline(display, x0+w1, y0+(2*hs)+3, w2,    white, 3);
    display_draw_hline(display, x0+w1, y0+(3*hs)+5, w2,    white, 3);
    display_draw_hline(display, x0+w1, y0+(4*hs)+7, w2,    white, 3);
    display_draw_hline(display, x0,    y0+h,        w1+w2, white, 3);
    display_draw_vline(display, x0,       y0, h, white, 3);
    display_draw_vline(display, x0+w1,    y0, h, white, 3);
    display_draw_vline(display, x0+w1+w2, y0, h, white, 3);

    display_draw_hline(display, x0,    y0,          w1+w2, black, 1);
    display_draw_hline(display, x0+w1, y0+(1*hs)+1, w2,    black, 1);
    display_draw_hline(display, x0+w1, y0+(2*hs)+3, w2,    black, 1);
    display_draw_hline(display, x0+w1, y0+(3*hs)+5, w2,    black, 1);
    display_draw_hline(display, x0+w1, y0+(4*hs)+7, w2,    black, 1);
    display_draw_hline(display, x0,    y0+h,        w1+w2, black, 1);
    display_draw_vline(display, x0,       y0, h, black, 1);
    display_draw_vline(display, x0+w1,    y0, h, black, 1);
    display_draw_vline(display, x0+w1+w2, y0, h, black, 1);
}
