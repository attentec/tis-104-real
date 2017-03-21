#include <stdlib.h>
#include "cpu.h"
#include "pipe.h"
#include "panic.h"

void cpu_init(struct cpu_t *cpu, struct prgm_t *prgm, struct state_t *state, struct pipe_t *inputs[], struct pipe_t *outputs[]) {
    cpu->prgm = prgm;
    cpu->state = state;
    for (size_t i = 0; i < CPU_MAX_PIPES; ++i) {
        cpu->inputs[i] = inputs[i];
        cpu->outputs[i] = outputs[i];
    }
}

static enum dir_t arg_to_dir(enum arg_t arg) {
    switch (arg) {
        case ARG_LEFT:
            return DIR_LEFT;
        case ARG_RIGHT:
            return DIR_RIGHT;
        case ARG_UP:
            return DIR_UP;
        case ARG_DOWN:
            return DIR_DOWN;
        default:
            break;
    }
    panic();
}

bool arg_is_dir(enum arg_t arg) {
    return arg == ARG_LEFT ||
        arg == ARG_RIGHT ||
        arg == ARG_UP ||
        arg == ARG_DOWN;
}

void cpu_read(struct cpu_t *cpu) {
    if (cpu->prgm->length == 0) {
        return;
    }

    addr_t *pc  = &cpu->state->pc;
    struct instr_t instr = cpu->prgm->instrs[*pc];

    if (arg_is_dir(instr.arg1)) {
        enum dir_t dir = arg_to_dir(instr.arg1);
        if (input_accept(cpu->inputs[dir], &cpu->state->rx)) {
            cpu->state->io_state = IO_STATE_RUNNING;
        } else {
            cpu->state->io_state = IO_STATE_BLOCKED_READ;
        }
    } else if (instr.arg1 == ARG_ANY) {
        if (input_accept(cpu->inputs[DIR_LEFT], &cpu->state->rx)) {
            cpu->state->io_state = IO_STATE_RUNNING;
        } else if (input_accept(cpu->inputs[DIR_RIGHT], &cpu->state->rx)) {
            cpu->state->io_state = IO_STATE_RUNNING;
        } else if (input_accept(cpu->inputs[DIR_UP], &cpu->state->rx)) {
            cpu->state->io_state = IO_STATE_RUNNING;
        } else if (input_accept(cpu->inputs[DIR_DOWN], &cpu->state->rx)) {
            cpu->state->io_state = IO_STATE_RUNNING;
        } else {
            cpu->state->io_state = IO_STATE_BLOCKED_READ;
        }
    }
}

void cpu_write(struct cpu_t *cpu) {
    if (cpu->prgm->length == 0) {
        return;
    }

    if (cpu->state->io_state == IO_STATE_BLOCKED_READ) {
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
    } else if (arg_is_dir(instr.arg1)) {
        arg1 = cpu->state->rx;
    } else if (instr.arg1 == ARG_ANY) {
        arg1 = cpu->state->rx;
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
    } else if (op == OP_MOV) {
        if (instr.arg2 == ARG_ACC) {
            cpu->state->acc = arg1;
        } else if (instr.arg2 == ARG_NIL) {
        } else if (arg_is_dir(instr.arg2)) {
            enum dir_t dir = arg_to_dir(instr.arg2);
            if (cpu->state->io_state == IO_STATE_BLOCKED_WRITE) {
                if (output_taken(cpu->outputs[dir])) {
                    cpu->state->io_state = IO_STATE_RUNNING;
                }
            } else {
                cpu->state->tx = arg1;
                output_offer(cpu->outputs[dir], &cpu->state->tx);
                *pc = original_pc;
                cpu->state->io_state = IO_STATE_BLOCKED_WRITE;
            }
        } else if (instr.arg2 == ARG_ANY) {
            if (cpu->state->io_state == IO_STATE_BLOCKED_WRITE) {
                if (output_taken(cpu->outputs[DIR_LEFT])
                        || output_taken(cpu->outputs[DIR_RIGHT])
                        || output_taken(cpu->outputs[DIR_UP])
                        || output_taken(cpu->outputs[DIR_DOWN])) {
                    cpu->state->io_state = IO_STATE_RUNNING;
                }
            } else {
                cpu->state->tx = arg1;
                output_offer(cpu->outputs[DIR_LEFT], &cpu->state->tx);
                output_offer(cpu->outputs[DIR_RIGHT], &cpu->state->tx);
                output_offer(cpu->outputs[DIR_UP], &cpu->state->tx);
                output_offer(cpu->outputs[DIR_DOWN], &cpu->state->tx);
                *pc = original_pc;
                cpu->state->io_state = IO_STATE_BLOCKED_WRITE;
            }
        }
    } else if (op == OP_ADD) {
        cpu->state->acc += arg1;
    }
}

void cpu_step(struct cpu_t *cpu) {
    cpu_read(cpu);
    cpu_write(cpu);
}
