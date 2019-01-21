#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "canvas.h"
#include "cpu.h"
#include "display.h"
#include "dispif.h"
#include "font.h"
#include "fonts.h"
#include "panic.h"

static void draw_static(struct canvas_t *canvas);
static void draw_borders(struct canvas_t *canvas);
static void draw_border_layer(struct canvas_t *canvas);
static void draw_status(struct canvas_t *canvas, struct state_t *cpu_state);
static void draw_labels(struct canvas_t *canvas);
static void draw_program(struct canvas_t *canvas, const char *lines[CPU_MAX_PRGM_LENGTH]);

const char *example_program_text[CPU_MAX_PRGM_LENGTH] = {
    "  MOV 10, ACC",
    "LOOP:",
    "  SUB 1",
    "  JNZ LOOP",
    "  SWP",
    "  ADD 1",
    "  SWP",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

int main(void)
{
    struct board_t board;
    struct display_t display;
    struct font_t font;
    struct canvas_t canvas;
    struct state_t cpu_state;

    board_init(&board);
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_Y_MAJOR);
    font_init(&font, monoblipp6x8);
    canvas_init(&canvas, &display, &font);
    cpu_state_init(&cpu_state);

    draw_static(&canvas);
    display_activate(&display);

    for (;;) {
        cpu_state.acc++;
        if (cpu_state.acc > 999) {
            cpu_state.acc = -999;
        }
        cpu_state.bak += 2;
        if (cpu_state.bak > 999) {
            cpu_state.bak = -999;
        }
        draw_status(&canvas, &cpu_state);
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
    canvas_set_fg_color(canvas, white);
    canvas_set_thickness(canvas, 3);
    draw_border_layer(canvas);

    canvas_set_fg_color(canvas, black);
    canvas_set_thickness(canvas, 1);
    draw_border_layer(canvas);
}

static void draw_border_layer(struct canvas_t *canvas)
{
    uint8_t x0 = main_x_pixels + 3;
    uint8_t w1 = char_width * (code_width_chars + 1);
    uint8_t w2 = char_width * (status_width_chars + 1);
    uint8_t y0 = main_y_pixels + 3;
    uint8_t h  = char_height * (code_height_chars + 1);
    uint8_t hs = char_height * (status_height_chars + 1);

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

static void draw_status(struct canvas_t *canvas, struct state_t *cpu_state)
{
    uint8_t x0 = main_x_pixels + (code_width_chars + 2) * char_width;
    uint8_t y0 = main_y_pixels + (char_height * 2);
    uint8_t w  = status_width_chars * char_width;
    uint8_t hs = ((status_height_chars + 1) * char_height) + 2;
    char buffer[7];
    const char *text;

    canvas_set_fg_color(canvas, white);
    canvas_set_bg_color(canvas, black);
    snprintf(buffer, 7, "%d", cpu_state->acc);
    canvas_draw_text(canvas, x0, y0+hs*0, w, ALIGN_CENTER, buffer);
    snprintf(buffer, 7, "(%d)", cpu_state->bak);
    canvas_draw_text(canvas, x0, y0+hs*1, w, ALIGN_CENTER, buffer);
    if (cpu_state->has_last) {
        switch (cpu_state->last) {
        case DIR_LEFT:
            text = "LEFT";
            break;
        case DIR_RIGHT:
            text = "RIGHT";
            break;
        case DIR_UP:
            text = "UP";
            break;
        case DIR_DOWN:
            text = "DOWN";
            break;
        default:
            panic();
            break;
        }
    } else {
        text = "N/A";
    }
    canvas_draw_text(canvas, x0, y0+hs*2, w, ALIGN_CENTER, text);
    canvas_draw_text(canvas, x0, y0+hs*3, w, ALIGN_CENTER, "IDLE");
    canvas_draw_text(canvas, x0, y0+hs*4, w, ALIGN_CENTER, "0%");
}

static void draw_program(struct canvas_t *canvas, const char *lines[CPU_MAX_PRGM_LENGTH])
{
    uint8_t x0 = main_x_pixels + char_width;
    uint8_t y0 = main_y_pixels + char_height;
    uint8_t w  = code_width_chars * char_width;

    canvas_set_fg_color(canvas, white);
    canvas_set_bg_color(canvas, black);
    for (uint8_t i = 0; i < CPU_MAX_PRGM_LENGTH; i++) {
        canvas_draw_text(canvas, x0, y0+(char_height*i), w, ALIGN_LEFT, lines[i]);
    }
}
