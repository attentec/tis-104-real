#pragma once

#include <inttypes.h>

struct font
{
    const uint8_t* data;
    uint8_t width;
    uint8_t height;
};

void font_init(struct font *font, const uint8_t *data);
uint8_t font_read_column(struct font *font, uint8_t charno, uint8_t column);
