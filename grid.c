#include "grid.h"
#include "cpu.h"
#include "pipe_mock.h"
#include <assert.h>
#include <stdlib.h>

#define CPU_RECT_WIDTH (4)
#define CPU_RECT_HEIGHT (3)
#define CPU_BORDER_WIDTH (CPU_RECT_WIDTH + 2)
#define CPU_BORDER_HEIGHT (CPU_RECT_HEIGHT + 2)
#define CPU_BORDER_SIZE (CPU_BORDER_WIDTH * CPU_BORDER_HEIGHT)
#define NUM_PIPES (CPU_BORDER_SIZE * CPU_MAX_PIPES)

_Static_assert(CPU_RECT_WIDTH * CPU_RECT_HEIGHT == GRID_MAX_NUM_CPUS,
               "CPU_RECT_WIDTH * CPU_RECT_HEIGHT must be sqrt(GRID_MAX_NUM_CPUS)");

static struct cpu_t cpus[GRID_MAX_NUM_CPUS];
struct prgm_t grid_prgms[GRID_MAX_NUM_CPUS];
struct state_t grid_states[GRID_MAX_NUM_CPUS];
static struct pipe_t pipes[NUM_PIPES];


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
            return self(i - CPU_BORDER_WIDTH);
        case DIR_DOWN:
            return self(i + CPU_BORDER_WIDTH);
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

static size_t index_in_border(size_t i) {
    size_t x = i % CPU_RECT_WIDTH;
    size_t y = i / CPU_RECT_WIDTH;
    return (x + 1) + CPU_BORDER_WIDTH * (y + 1);
}

static void read(void) {
    for (size_t i = 0; i < GRID_MAX_NUM_CPUS; ++i) {
        cpu_read(&cpus[i]);
    }
}

static void write(void) {
    for (size_t i = 0; i < GRID_MAX_NUM_CPUS; ++i) {
        cpu_write(&cpus[i]);
    }
}

void grid_init(void) {
    struct pipe_t *input_pointers[CPU_MAX_PIPES];
    struct pipe_t *output_pointers[CPU_MAX_PIPES];

    for (size_t i = 0; i < NUM_PIPES; ++i) {
        pipes[i].cell = NULL;
    }

    for (size_t i = 0; i < GRID_MAX_NUM_CPUS; ++i) {
        for (enum dir_t d = DIR_MIN; d <= DIR_MAX; ++d) {
            size_t ib = index_in_border(i);
            output_pointers[d] = &pipes[output_index(ib, d)];
            input_pointers[d] = &pipes[input_index(ib, d)];
        }

        cpu_state_init(&grid_states[i]);
        cpu_init(&cpus[i], &grid_prgms[i], &grid_states[i], input_pointers, output_pointers);
    }
}

void grid_step(void) {
    read();
    write();
}
