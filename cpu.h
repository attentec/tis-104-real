#pragma once

#include <stdint.h>

#define CPU_MAX_PRGM_LENGTH (15)
#define CPU_MAX_PIPES (4)

typedef uint8_t addr_t;
typedef int16_t reg_t;

enum op_t { OP_NOP,
    OP_MOV,
    OP_SWP,
    OP_SAV,
    OP_ADD,
    OP_SUB,
    OP_NEG,
    OP_JMP,
    OP_JEZ,
    OP_JNZ,
    OP_JGZ,
    OP_JLZ,
    OP_JRO,
};

enum arg_t {
    ARG_NONE = -1000,
    ARG_ACC = 1000,
    ARG_NIL,
    ARG_LEFT,
    ARG_RIGHT,
    ARG_UP,
    ARG_DOWN,
    ARG_ANY,
    ARG_LAST,
};

enum dir_t {
    DIR_LEFT = 0,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
};

struct instr_t {
    enum op_t op;
    enum arg_t arg1;
    enum arg_t arg2;
};

struct prgm_t {
    addr_t length;
    struct instr_t instrs[CPU_MAX_PRGM_LENGTH];
};

struct state_t {
    addr_t pc;
    reg_t acc;
    reg_t bak;
};

struct cpu_t {
    struct prgm_t *prgm;
    struct state_t *state;
    struct pipe_t *inputs[CPU_MAX_PIPES];
    struct pipe_t *outputs[CPU_MAX_PIPES];
};

void cpu_init(struct cpu_t *cpu, struct prgm_t *prgm, struct state_t *state, struct pipe_t *inputs[], struct pipe_t *outputs[]);
void cpu_read(struct cpu_t *cpu);
void cpu_write(struct cpu_t *cpu);
void cpu_step(struct cpu_t *cpu);
