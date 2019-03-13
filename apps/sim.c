#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "canvas.h"
#include "code.h"
#include "cpu.h"
#include "display.h"
#include "dispif.h"
#include "gui.h"
#include "font.h"
#include "fonts.h"
#include "icons.h"
#include "panic.h"

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
    struct display_t display;
    struct canvas_t canvas;
    struct code_t code;
    struct state_t cpu_state;
    struct cpu_t cpu;

    board_init();
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_Y_MAJOR);
    canvas_init(&canvas, &display, &monoblipp6x8);
    cpu_state_init(&cpu_state);
    code_init(&code, example_program_text);
    cpu_init(&cpu, &code.prgm, &cpu_state);

    gui_show_cpu(&canvas, code.lines);
    display_activate(&display);

    address_t last_line = code.addr_to_line[0];
    for (;;) {
        uint8_t current_line = code.addr_to_line[cpu_state.pc];
        gui_update_cpu(&canvas, code.lines, &cpu_state, last_line, current_line);
        last_line = current_line;
        cpu_step(&cpu);
    }

    return 0;
}
