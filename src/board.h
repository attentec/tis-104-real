#pragma once

struct dispif_t;

struct board_t {
    struct dispif_t *dispif;
};

extern struct board_t board;

void board_init(void);
