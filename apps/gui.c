#include <stdint.h>

#include "board.h"
#include "canvas.h"
#include "display.h"
#include "dispif.h"
#include "font.h"
#include "fonts.h"

static void draw_static(struct canvas_t *canvas);
static void draw_borders(struct canvas_t *canvas);
static void draw_labels(struct canvas_t *canvas);
static void draw_status(struct canvas_t *canvas);

int main(void)
{
    struct board_t board;
    struct display_t display;
    struct font_t font;
    struct canvas_t canvas;

    board_init(&board);
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_Y_MAJOR);
    font_init(&font, monoblipp6x8);
    canvas_init(&canvas, &display, &font);

    display_activate(&display);
    draw_static(&canvas);

    for (;;) {
        draw_status(&canvas);
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
static uint16_t gray  = RGB888_TO_RGB565(0xAAAAAAul);
static uint16_t black = RGB888_TO_RGB565(0x000000ul);

static void draw_static(struct canvas_t *canvas)
{
    canvas_clear(canvas, black);
    draw_borders(canvas);
    draw_labels(canvas);
}

static void draw_borders(struct canvas_t *canvas)
{

    uint8_t x0 = main_x_pixels + 3;
    uint8_t w1 = char_width * (code_width_chars + 1);
    uint8_t w2 = char_width * (status_width_chars + 1);
    uint8_t y0 = main_y_pixels + 3;
    uint8_t h  = char_height * (code_height_chars + 1);
    uint8_t hs = char_height * (status_height_chars + 1);

    canvas_set_fg_color(canvas, white);
    canvas_set_thickness(canvas, 3);
    canvas_draw_hline(canvas, x0,    y0,          w1+w2);
    canvas_draw_hline(canvas, x0+w1, y0+(1*hs)+1, w2);
    canvas_draw_hline(canvas, x0+w1, y0+(2*hs)+3, w2);
    canvas_draw_hline(canvas, x0+w1, y0+(3*hs)+5, w2);
    canvas_draw_hline(canvas, x0+w1, y0+(4*hs)+7, w2);
    canvas_draw_hline(canvas, x0,    y0+h,        w1+w2);
    canvas_draw_vline(canvas, x0,       y0, h);
    canvas_draw_vline(canvas, x0+w1,    y0, h);
    canvas_draw_vline(canvas, x0+w1+w2, y0, h);

    canvas_set_fg_color(canvas, black);
    canvas_set_thickness(canvas, 1);
    canvas_draw_hline(canvas, x0,    y0,          w1+w2);
    canvas_draw_hline(canvas, x0+w1, y0+(1*hs)+1, w2);
    canvas_draw_hline(canvas, x0+w1, y0+(2*hs)+3, w2);
    canvas_draw_hline(canvas, x0+w1, y0+(3*hs)+5, w2);
    canvas_draw_hline(canvas, x0+w1, y0+(4*hs)+7, w2);
    canvas_draw_hline(canvas, x0,    y0+h,        w1+w2);
    canvas_draw_vline(canvas, x0,       y0, h);
    canvas_draw_vline(canvas, x0+w1,    y0, h);
    canvas_draw_vline(canvas, x0+w1+w2, y0, h);
}

static void draw_labels(struct canvas_t *canvas)
{
    uint8_t x0 = main_x_pixels + (code_width_chars + 2) * char_width;
    uint8_t y0 = main_y_pixels + char_height;
    uint8_t w  = status_width_chars * char_width;
    uint8_t hs = ((status_height_chars + 1) * char_height) + 2;

    canvas_set_fg_color(canvas, gray);
    canvas_set_bg_color(canvas, black);
    canvas_draw_text(canvas, x0, y0+hs*0, w, ALIGN_CENTER, "ACC");
    canvas_draw_text(canvas, x0, y0+hs*1, w, ALIGN_CENTER, "BAK");
    canvas_draw_text(canvas, x0, y0+hs*2, w, ALIGN_CENTER, "LAST");
    canvas_draw_text(canvas, x0, y0+hs*3, w, ALIGN_CENTER, "MODE");
    canvas_draw_text(canvas, x0, y0+hs*4, w, ALIGN_CENTER, "IDLE");
}

static void draw_status(struct canvas_t *canvas)
{
    uint8_t x0 = main_x_pixels + (code_width_chars + 2) * char_width;
    uint8_t y0 = main_y_pixels + (char_height * 2);
    uint8_t w  = status_width_chars * char_width;
    uint8_t hs = ((status_height_chars + 1) * char_height) + 2;

    canvas_set_fg_color(canvas, white);
    canvas_set_bg_color(canvas, black);
    canvas_draw_text(canvas, x0, y0+hs*0, w, ALIGN_CENTER, "0");
    canvas_draw_text(canvas, x0, y0+hs*1, w, ALIGN_CENTER, "(0)");
    canvas_draw_text(canvas, x0, y0+hs*2, w, ALIGN_CENTER, "N/A");
    canvas_draw_text(canvas, x0, y0+hs*3, w, ALIGN_CENTER, "IDLE");
    canvas_draw_text(canvas, x0, y0+hs*4, w, ALIGN_CENTER, "0%");
}