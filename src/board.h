#pragma once

struct dispif_t;

struct board_t {
    struct dispif_t *dispif;
};

void board_init(struct board_t *board);
