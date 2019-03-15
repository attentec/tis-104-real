#pragma once

#include "display.h"

struct dispif_t;

enum orientation_t {
    ORIENTATION_RIBBON_BOTTOM,
    ORIENTATION_RIBBON_LEFT,
    ORIENTATION_RIBBON_TOP,
    ORIENTATION_RIBBON_RIGHT
};

struct display_t {
    struct dispif_t *dispif;
    enum orientation_t orientation;
};

void display_ili9225_init(struct display_t *display, struct dispif_t *dispif, enum orientation_t orientation);
