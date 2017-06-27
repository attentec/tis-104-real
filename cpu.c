#include <stddef.h>

#include "cpu.h"
#include "panic.h"
#include "pipe.h"

enum pc_action_t {
    PC_ACTION_INCREMENT = 0,
    PC_ACTION_BLOCK,
    PC_ACTION_JUMP,
};

void cpu_state_init(struct state_t *state) {
    state->pc = 0;
    state->acc = 0;
    state->bak = 0;
    state->rx = REG_INVALID_VALUE;
    state->tx = REG_INVALID_VALUE;
    state->has_last = false;
    state->last = DIR_LEFT;
    state->io_state = IO_STATE_RUNNING;
}

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

static enum arg_t dir_to_arg(enum dir_t dir) {
    switch (dir) {
        case DIR_LEFT:
            return ARG_LEFT;
        case DIR_RIGHT:
            return ARG_RIGHT;
        case DIR_UP:
            return ARG_UP;
        case DIR_DOWN:
            return ARG_DOWN;
        default:
            break;
    }
    panic();
}

static bool dir_matches(enum dir_t dir, enum arg_t arg) {
    return arg == ARG_ANY
        || arg_to_dir(arg) == dir;
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

    address_t *pc  = &cpu->state->pc;
    struct instr_t instr = cpu->prgm->instrs[*pc];

    if (arg_is_dir(instr.arg1)) {
        enum dir_t dir = arg_to_dir(instr.arg1);
        if (input_accept(cpu->inputs[dir], &cpu->state->rx)) {
            cpu->state->io_state = IO_STATE_RUNNING;
        } else {
            cpu->state->io_state = IO_STATE_BLOCKED_READ;
        }
    } else if (instr.arg1 == ARG_ANY) {
        bool got_input = false;
        for (enum dir_t d = DIR_MIN; d <= DIR_MAX; ++d) {
            if (input_accept(cpu->inputs[d], &cpu->state->rx)) {
                got_input = true;
                cpu->state->has_last = true;
                cpu->state->last = d;
                break;
            }
        }
        cpu->state->io_state = got_input ? IO_STATE_RUNNING : IO_STATE_BLOCKED_READ;
    } else if (instr.arg1 == ARG_LAST) {
        if (cpu->state->has_last
                && !input_accept(cpu->inputs[cpu->state->last], &cpu->state->rx)) {
            cpu->state->io_state = IO_STATE_BLOCKED_READ;
        } else {
            cpu->state->io_state = IO_STATE_RUNNING;
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

    address_t *pc  = &cpu->state->pc;
    reg_t  *acc = &cpu->state->acc;
    reg_t  *bak = &cpu->state->bak;
    struct instr_t instr = cpu->prgm->instrs[*pc];
    address_t length = cpu->prgm->length;
    enum op_t op = instr.op;

    reg_t arg1;
    if (instr.arg1 == ARG_ACC) {
        arg1 = cpu->state->acc;
    } else if (instr.arg1 == ARG_NIL) {
        arg1 = 0;
    } else if (arg_is_dir(instr.arg1)) {
        arg1 = cpu->state->rx;
    } else if (instr.arg1 == ARG_ANY) {
        arg1 = cpu->state->rx;
    } else if (instr.arg1 == ARG_LAST) {
        if (cpu->state->has_last) {
            arg1 = cpu->state->rx;
        } else {
            arg1 = 0;
        }
    } else {
        arg1 = (reg_t) instr.arg1;
    }

    enum pc_action_t pc_action = PC_ACTION_INCREMENT;
    address_t jump_pc = 0;

    if (op == OP_SWP) {
        reg_t temp = *acc;
        *acc = *bak;
        *bak = temp;
    } else if (op == OP_SAV) {
        *bak = *acc;
    } else if (op == OP_NEG) {
        *acc = -*acc;
    } else if (op == OP_JMP) {
        pc_action = PC_ACTION_JUMP;
        jump_pc = (address_t) arg1;
    } else if (op == OP_JEZ) {
        if (*acc == 0) {
            pc_action = PC_ACTION_JUMP;
            jump_pc = (address_t) arg1;
        }
    } else if (op == OP_JNZ) {
        if (*acc != 0) {
            pc_action = PC_ACTION_JUMP;
            jump_pc = (address_t) arg1;
        }
    } else if (op == OP_JGZ) {
        if (*acc > 0) {
            pc_action = PC_ACTION_JUMP;
            jump_pc = (address_t) arg1;
        }
    } else if (op == OP_JLZ) {
        if (*acc < 0) {
            pc_action = PC_ACTION_JUMP;
            jump_pc = (address_t) arg1;
        }
    } else if (op == OP_JRO) {
        reg_t offset = arg1;
        reg_t unclamped_pc = (reg_t) *pc + offset;
        if (unclamped_pc < 0) {
            pc_action = PC_ACTION_JUMP;
            jump_pc = 0;
        } else if (unclamped_pc >= (reg_t) length) {
            pc_action = PC_ACTION_JUMP;
            jump_pc = length - 1;
        } else {
            pc_action = PC_ACTION_JUMP;
            jump_pc = (address_t) unclamped_pc;
        }
    } else if (op == OP_MOV) {
        if (instr.arg2 == ARG_ACC) {
            cpu->state->acc = arg1;
        } else if (instr.arg2 == ARG_NIL) {
        } else if (arg_is_dir(instr.arg2) || instr.arg2 == ARG_ANY || instr.arg2 == ARG_LAST) {
            enum arg_t arg2 = instr.arg2;
            if (instr.arg2 == ARG_LAST && cpu->state->has_last) {
                arg2 = dir_to_arg(cpu->state->last);
            }
            if (instr.arg2 == ARG_LAST && !cpu->state->has_last) {
                pc_action = PC_ACTION_INCREMENT;
            } else if (cpu->state->io_state == IO_STATE_BLOCKED_WRITE) {
                pc_action = PC_ACTION_BLOCK;
                for (enum dir_t d = DIR_MIN; d <= DIR_MAX; ++d) {
                    if (dir_matches(d, arg2) && output_taken(cpu->outputs[d])) {
                        if (output_used(cpu->outputs[d])) {
                            cpu->state->last = d;
                            cpu->state->has_last = true;
                        }
                        output_abstain(cpu->outputs[d]);
                        cpu->state->io_state = IO_STATE_RUNNING;
                        pc_action = PC_ACTION_INCREMENT;
                    }
                }
            } else {
                cpu->state->tx = arg1;
                pc_action = PC_ACTION_BLOCK;
                cpu->state->io_state = IO_STATE_BLOCKED_WRITE;
                for (enum dir_t d = DIR_MIN; d <= DIR_MAX; ++d) {
                    if (dir_matches(d, arg2)) {
                        output_offer(cpu->outputs[d], &cpu->state->tx);
                    }
                }
            }
        }
    } else if (op == OP_ADD) {
        cpu->state->acc += arg1;
    } else if (op == OP_SUB) {
        cpu->state->acc -= arg1;
    } else if (op == OP_NOP) {
    } else {
        panic();
    }

    if (pc_action == PC_ACTION_INCREMENT) {
        *pc = (*pc + 1) % length;
    } else if (pc_action == PC_ACTION_JUMP) {
        *pc = jump_pc;
    }
}

void cpu_step(struct cpu_t *cpu) {
    cpu_read(cpu);
    cpu_write(cpu);
}
