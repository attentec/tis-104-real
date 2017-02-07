#pragma once

#include <stdint.h>

#define CPU_MAX_PRGM_LENGTH 15

typedef uint8_t addr_t;
typedef int16_t reg_t;

enum op_t {
    OP_NOP,
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
};

void cpu_init(struct cpu_t *cpu, struct prgm_t *prgm, struct state_t *state);
void cpu_step(struct cpu_t *cpu);
