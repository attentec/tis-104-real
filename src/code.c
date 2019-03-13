#include "code.h"

void code_init(struct code_t *code, const char *lines[CPU_MAX_PRGM_LENGTH])
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
