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

    addr_t *pc  = &cpu->state->pc;
    reg_t  *acc = &cpu->state->acc;
    reg_t  *bak = &cpu->state->bak;
    struct instr_t instr = cpu->prgm->instrs[*pc];
    addr_t length = cpu->prgm->length;
    enum op_t op = instr.op;
    addr_t original_pc = *pc;

    reg_t arg1;
    if (instr.arg1 == ARG_ACC) {
        arg1 = cpu->state->acc;
    } else if (instr.arg1 == ARG_NIL) {
        arg1 = 0;
    } else {
        arg1 = (reg_t) instr.arg1;
    }

    *pc = (original_pc + 1) % length;

    if (op == OP_SWP) {
        reg_t temp = *acc;
        *acc = *bak;
        *bak = temp;
    } else if (op == OP_SAV) {
        *bak = *acc;
    } else if (op == OP_NEG) {
        *acc = -*acc;
    } else if (op == OP_JMP) {
        *pc = (addr_t) arg1;
    } else if (op == OP_JEZ) {
        if (*acc == 0) {
            *pc = (addr_t) arg1;
        }
    } else if (op == OP_JNZ) {
        if (*acc != 0) {
            *pc = (addr_t) arg1;
        }
    } else if (op == OP_JGZ) {
        if (*acc > 0) {
            *pc = (addr_t) arg1;
        }
    } else if (op == OP_JLZ) {
        if (*acc < 0) {
            *pc = (addr_t) arg1;
        }
    } else if (op == OP_JRO) {
        reg_t offset = arg1;
        reg_t unclamped_pc = (reg_t) original_pc + offset;
        if (unclamped_pc < 0) {
            *pc = 0;
        } else if (unclamped_pc >= (reg_t) length) {
            *pc = length - 1;
        } else {
            *pc = (addr_t) unclamped_pc;
        }
    }
}
