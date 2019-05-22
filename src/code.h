#pragma once

#include <stdint.h>

#include "cpu.h"

#define CODE_NUM_LINES (15)
#define CODE_LINE_MAX_CHARS (18)
#define CODE_LINE_BUFFER_SIZE (CODE_LINE_MAX_CHARS + 1)

typedef char source_t[CODE_NUM_LINES][CODE_LINE_BUFFER_SIZE];

struct code_t {
    struct prgm_t prgm;
    const source_t *lines;
    uint8_t addr_to_line[CPU_MAX_PRGM_LENGTH];
    bool errors[CODE_NUM_LINES];
};

void code_init(struct code_t *code, const source_t *lines);
