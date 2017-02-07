#include "cpu.h"

void cpu_init(struct cpu_t *cpu, struct prgm_t *prgm, struct state_t *state) {
    cpu->prgm = prgm;
    cpu->state = state;
}

void cpu_step(struct cpu_t *cpu) {
    if (cpu->prgm->length == 0) {
        return;
    }
    cpu->state->pc = (cpu->state->pc + 1) % cpu->prgm->length;
}