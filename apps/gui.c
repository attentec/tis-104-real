#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "canvas.h"
#include "cpu.h"
#include "display.h"
#include "dispif.h"
#include "font.h"
#include "fonts.h"
#include "icons.h"
#include "panic.h"
#include "pipe_mock.h"

struct code_t {
    struct prgm_t prgm;
    const char **lines;
    uint8_t addr_to_line[CPU_MAX_PRGM_LENGTH];
};

static void compile(struct code_t *code, const char *lines[CPU_MAX_PRGM_LENGTH]);
static void setup_pipes(struct pipe_t inputs[], struct pipe_t outputs[], struct pipe_t *input_ptrs[], struct pipe_t *output_ptrs[]);

static void draw_static(struct canvas_t *canvas, struct code_t *code);
static void draw_borders(struct canvas_t *canvas);
static void draw_border_layer(struct canvas_t *canvas);
static void draw_arrows(struct canvas_t *canvas);
static void draw_status(struct canvas_t *canvas, struct state_t *cpu_state);
static void draw_labels(struct canvas_t *canvas);
static void draw_program(struct canvas_t *canvas, struct code_t *code);
static void draw_program_highlight(struct canvas_t *canvas, struct code_t *code, address_t last_pc, address_t current_pc);

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
    struct canvas_t canvas;
    struct code_t code;
    struct state_t cpu_state;
    struct pipe_t inputs[CPU_MAX_PIPES];
    struct pipe_t outputs[CPU_MAX_PIPES];
    struct pipe_t *input_ptrs[CPU_MAX_PIPES];
    struct pipe_t *output_ptrs[CPU_MAX_PIPES];
    struct cpu_t cpu;

    board_init(&board);
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_Y_MAJOR);
    canvas_init(&canvas, &display, &monoblipp6x8);
    cpu_state_init(&cpu_state);
    compile(&code, example_program_text);
    setup_pipes(inputs, outputs, input_ptrs, output_ptrs);
    cpu_init(&cpu, &code.prgm, &cpu_state, input_ptrs, output_ptrs);

    draw_static(&canvas, &code);
    display_activate(&display);

    address_t last_pc = cpu_state.pc;
    for (;;) {
        draw_status(&canvas, &cpu_state);
        draw_program_highlight(&canvas, &code, last_pc, cpu_state.pc);
        last_pc = cpu_state.pc;
        cpu_step(&cpu);
    }

    return 0;
}

static void compile(struct code_t *code, const char *lines[CPU_MAX_PRGM_LENGTH])
{
    *code = (struct code_t) {
        .prgm = {
            .length = 6,
            .instrs = {
                INSTR2(OP_MOV, 10, ARG_ACC),
                INSTR1(OP_SUB, 1),
                INSTR1(OP_JNZ, 1),
                INSTR0(OP_SWP),
                INSTR1(OP_ADD, 1),
                INSTR0(OP_SWP)
            }
        },
        .lines = lines,
        .addr_to_line = {
            0,
            2,
            3,
            4,
            5,
            6
        }
    };
}

static void setup_pipes(struct pipe_t inputs[], struct pipe_t outputs[], struct pipe_t *input_ptrs[], struct pipe_t *output_ptrs[])
{
   for (uint8_t i = 0; i < CPU_MAX_PIPES; ++i) {
        inputs[i].cell = NULL;
        inputs[i].used = false;
        outputs[i].cell = NULL;
        outputs[i].used = false;
        input_ptrs[i] = &inputs[i];
        output_ptrs[i] = &outputs[i];
    }
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

static void draw_static(struct canvas_t *canvas, struct code_t *code)
{
    canvas_clear(canvas, black);
    draw_borders(canvas);
    draw_arrows(canvas);
    draw_labels(canvas);
    draw_program(canvas, code);
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

static void draw_arrows(struct canvas_t *canvas)
{
    canvas_set_fg_color(canvas, white);
    canvas_set_bg_color(canvas, black);
    canvas_draw_icon(canvas,   6,  71, ROT_270, &arrow_icon);
    canvas_draw_icon(canvas,   6,  95, ROT_90,  &arrow_icon);
    canvas_draw_icon(canvas, 198,  71, ROT_270, &arrow_icon);
    canvas_draw_icon(canvas, 198,  95, ROT_90,  &arrow_icon);
    canvas_draw_icon(canvas,  88,   0, ROT_180, &arrow_icon);
    canvas_draw_icon(canvas, 118,   0, ROT_0,   &arrow_icon);
    canvas_draw_icon(canvas,  88, 160, ROT_180, &arrow_icon);
    canvas_draw_icon(canvas, 118, 160, ROT_0,   &arrow_icon);
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

static void draw_program(struct canvas_t *canvas, struct code_t *code)
{
    uint8_t x0 = main_x_pixels + char_width;
    uint8_t y0 = main_y_pixels + char_height;
    uint8_t w  = code_width_chars * char_width;

    canvas_set_fg_color(canvas, white);
    canvas_set_bg_color(canvas, black);
    for (uint8_t i = 0; i < CPU_MAX_PRGM_LENGTH; i++) {
        canvas_draw_text(canvas, x0, y0+(char_height*i), w, ALIGN_LEFT, code->lines[i]);
    }
}

static void draw_program_highlight(struct canvas_t *canvas, struct code_t *code, address_t last_pc, address_t current_pc)
{
    uint8_t last_line = code->addr_to_line[last_pc];
    uint8_t current_line = code->addr_to_line[current_pc];
    uint8_t x0 = main_x_pixels + char_width;
    uint8_t y0 = main_y_pixels + char_height;
    uint8_t w  = code_width_chars * char_width;

    canvas_set_fg_color(canvas, white);
    canvas_set_bg_color(canvas, black);
    canvas_draw_text(canvas, x0, y0+(char_height*last_line), w, ALIGN_LEFT, code->lines[last_line]);
    canvas_set_fg_color(canvas, black);
    canvas_set_bg_color(canvas, white);
    canvas_draw_text(canvas, x0, y0+(char_height*current_line), w, ALIGN_LEFT, code->lines[current_line]);
}
