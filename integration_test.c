#include "unity.h"
#include "cpu.h"
#include "pipe_mock.h"
#include <assert.h>

#define INSTR0(op)             ((struct instr_t){(op), ARG_NONE, ARG_NONE})
#define INSTR1(op, arg1)       ((struct instr_t){(op), (arg1),   ARG_NONE})
#define INSTR2(op, arg1, arg2) ((struct instr_t){(op), (arg1),   (arg2)})

enum {
    CPU_UL = 0,
    CPU_UR,
    CPU_LL,
    CPU_LR,
    NUM_CPUS,
};

#define CPU_SQUARE_SIDE (2)
#define CPU_BORDER_SIDE (CPU_SQUARE_SIDE + 2)
#define CPU_BORDER_SIZE (CPU_BORDER_SIDE * CPU_BORDER_SIDE)

_Static_assert(CPU_SQUARE_SIDE * CPU_SQUARE_SIDE == NUM_CPUS,
               "CPU_SQUARE_SIDE must be sqrt(NUM_CPUS)");

static struct cpu_t cpus[NUM_CPUS];
static struct prgm_t prgms[NUM_CPUS];
static struct state_t states[NUM_CPUS];
static struct pipe_t pipes[CPU_BORDER_SIZE * CPU_MAX_PIPES];


static size_t self(size_t i) {
    return i * CPU_MAX_PIPES;
}

static size_t neighbor(size_t i, enum dir_t dir) {
    switch (dir) {
        case DIR_LEFT:
            return self(i - 1);
        case DIR_RIGHT:
            return self(i + 1);
        case DIR_UP:
            return self(i - CPU_SQUARE_SIDE);
        case DIR_DOWN:
            return self(i + CPU_SQUARE_SIDE);
        default:
            break;
    }
    assert(false);
}

static size_t invert_dir(enum dir_t dir) {
    switch (dir) {
        case DIR_LEFT:
            return DIR_RIGHT;
        case DIR_RIGHT:
            return DIR_LEFT;
        case DIR_UP:
            return DIR_DOWN;
        case DIR_DOWN:
            return DIR_UP;
        default:
            break;
    }
    assert(false);
}

static size_t output_index(size_t i, enum dir_t dir) {
    return self(i) + (size_t)dir;
}

static size_t input_index(size_t i, enum dir_t dir) {
    return neighbor(i, dir) + invert_dir(dir);
}

static void read(void) {
    for (size_t i = 0; i < NUM_CPUS; ++i) {
        cpu_read(&cpus[i]);
    }
}

static void write(void) {
    for (size_t i = 0; i < NUM_CPUS; ++i) {
        cpu_write(&cpus[i]);
    }
}

static void step(void) {
    read();
    write();
}

void setUp(void) {
    struct pipe_t *input_pointers[CPU_MAX_PIPES];
    struct pipe_t *output_pointers[CPU_MAX_PIPES];

    for (size_t i = 0; i < NUM_CPUS; ++i) {
        for (enum dir_t d = DIR_MIN; d <= DIR_MAX; ++d) {
            output_pointers[d] = &pipes[output_index(i, d)];
            input_pointers[d] = &pipes[input_index(i, d)];
        }

        states[i].pc = 0;
        states[i].acc = 0;
        states[i].bak = 0;
        states[i].rx = REG_INVALID_VALUE;

        cpu_init(&cpus[i], &prgms[i], &states[i], input_pointers, output_pointers);
    }
}

void test_Integration_should_FollowBasicCommunicationTiming(void) {
    prgms[CPU_UL].length = 2;
    prgms[CPU_UL].instrs[0] = INSTR2(OP_MOV, 1, ARG_RIGHT);
    prgms[CPU_UL].instrs[1] = INSTR1(OP_ADD, ARG_RIGHT);
    prgms[CPU_UR].length = 1;
    prgms[CPU_UR].instrs[0] = INSTR2(OP_MOV, ARG_LEFT, ARG_LEFT);
    prgms[CPU_LL].length = 0;
    prgms[CPU_LR].length = 0;

    states[CPU_UL].acc = 0;

    step();
    step();
    step();

    TEST_ASSERT_EQUAL_INT(1, states[CPU_UL].acc);
}
