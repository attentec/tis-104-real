#pragma once

struct screen_t;
struct dispif_t;
struct tft_t;

struct board_t {
    struct screen_t *scr;
    struct dispif_t *dispif;
    struct tft_t *tft;
};

void board_init(struct board_t *board);
