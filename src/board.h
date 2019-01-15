#pragma once

#include "screen.h"
#include "disp.h"
#include "tft.h"

struct board_t {
    struct screen *scr;
    struct disp_t *disp;
    struct tft_t *tft;
};

void board_init(struct board_t *board);
