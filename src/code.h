#pragma once

#include <stdint.h>

#include "cpu.h"

struct code_t {
    struct prgm_t prgm;
    const char **lines;
    uint8_t addr_to_line[CPU_MAX_PRGM_LENGTH];
    bool errors[CPU_MAX_PRGM_LENGTH];
};

void code_init(struct code_t *code, const char *lines[CPU_MAX_PRGM_LENGTH]);
