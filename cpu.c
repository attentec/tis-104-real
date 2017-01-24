#include "cpu.h"

void cpu_init(struct cpu_t *cpu, struct prgm_t *prgm, struct state_t *state) {
    cpu->state = state;
    (void) prgm;
}

void cpu_step(struct cpu_t *cpu) {
    cpu->state->pc = 1;
}
