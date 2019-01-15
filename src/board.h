#pragma once

#include "screen.h"
#include "dispif.h"
#include "tft.h"

struct board_t {
    struct screen_t *scr;
    struct dispif_t *dispif;
    struct tft_t *tft;
};

void board_init(struct board_t *board);
