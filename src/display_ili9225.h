#pragma once

#include "display.h"
#include "platform.h"

struct dispif_t;

struct display_t {
    struct dispif_t *dispif;
    enum orientation_t orientation;
};

void display_ili9225_init(struct display_t *display, struct dispif_t *dispif, enum orientation_t orientation);
