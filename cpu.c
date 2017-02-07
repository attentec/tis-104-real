#include "cpu.h"
#include <stdio.h>

void cpu_init(struct cpu_t *cpu, struct prgm_t *prgm, struct state_t *state) {
    cpu->prgm = prgm;
    cpu->state = state;
}

void cpu_step(struct cpu_t *cpu) {
    if (cpu->prgm->length == 0) {
        return;
    }
    struct instr_t instr = cpu->prgm->instrs[cpu->state->pc];
    if (instr.op == OP_JMP) {
        if (instr.arg1 < 0) {
            cpu->state->pc = 0;
        } else if (instr.arg1 >= cpu->prgm->length) {
            cpu->state->pc = cpu->prgm->length - 1;
        } else {
            cpu->state->pc = (addr_t) instr.arg1;
        }
    } else {
        cpu->state->pc = (cpu->state->pc + 1) % cpu->prgm->length;
    }
}
