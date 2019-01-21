#pragma once

#define GRID_MAX_NUM_CPUS 12

void grid_init(void);
void grid_step(void);

extern struct prgm_t grid_prgms[GRID_MAX_NUM_CPUS];
extern struct state_t grid_states[GRID_MAX_NUM_CPUS];
