#pragma once

struct dispif_t;
struct pin_t;

struct board_t {
    struct dispif_t *dispif;
    struct pin_t *led;
};

extern struct board_t board;

void board_init(void);
