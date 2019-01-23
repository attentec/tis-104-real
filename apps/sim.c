#include <stdint.h>
#include <stdio.h>

#include "board.h"
#include "canvas.h"
#include "cpu.h"
#include "display.h"
#include "dispif.h"
#include "gui.h"
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
    struct pipe_t inputs[CPU_MAX_PIPES];
    struct pipe_t outputs[CPU_MAX_PIPES];
    struct pipe_t *input_ptrs[CPU_MAX_PIPES];
    struct pipe_t *output_ptrs[CPU_MAX_PIPES];
    struct cpu_t cpu;

    board_init();
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_Y_MAJOR);
    canvas_init(&canvas, &display, &monoblipp6x8);
    cpu_state_init(&cpu_state);
    compile(&code, example_program_text);
    setup_pipes(inputs, outputs, input_ptrs, output_ptrs);
    cpu_init(&cpu, &code.prgm, &cpu_state, input_ptrs, output_ptrs);

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
        pipe_init(&inputs[i]);
        pipe_init(&outputs[i]);
        input_ptrs[i] = &inputs[i];
        output_ptrs[i] = &outputs[i];
    }
}
