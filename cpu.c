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
    addr_t original_pc = cpu->state->pc;

    reg_t arg1;
    if (instr.arg1 == ARG_ACC) {
        arg1 = cpu->state->acc;
    } else {
        arg1 = (reg_t) instr.arg1;
    }

    cpu->state->pc = (original_pc + 1) % cpu->prgm->length;

    if (instr.op == OP_SWP) {
        reg_t temp = cpu->state->acc;
        cpu->state->acc = cpu->state->bak;
        cpu->state->bak = temp;
    } else if (instr.op == OP_SAV) {
        cpu->state->bak = cpu->state->acc;
    } else if (instr.op == OP_JRO) {
        reg_t offset = arg1;
        reg_t unclamped_pc = (reg_t) original_pc + offset;
        if (unclamped_pc < 0) {
            cpu->state->pc = 0;
        } else if (unclamped_pc >= (reg_t) cpu->prgm->length) {
            cpu->state->pc = cpu->prgm->length - 1;
        } else {
            cpu->state->pc = (addr_t) unclamped_pc;
        }
    } else if (instr.op == OP_JMP) {
        cpu->state->pc = (addr_t) arg1;
    } else if (instr.op == OP_JEZ) {
        if (cpu->state->acc == 0) {
            cpu->state->pc = (addr_t) arg1;
        }
    } else if (instr.op == OP_JNZ) {
        if (cpu->state->acc != 0) {
            cpu->state->pc = (addr_t) arg1;
        }
    } else if (instr.op == OP_JGZ) {
        if (cpu->state->acc > 0) {
            cpu->state->pc = (addr_t) arg1;
        }
    } else if (instr.op == OP_JLZ) {
        if (cpu->state->acc < 0) {
            cpu->state->pc = (addr_t) arg1;
        }
    }
}
