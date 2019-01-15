#pragma once

#include <stdint.h>

struct font_t
{
    const uint8_t* data;
    uint8_t width;
    uint8_t height;
};

void font_init(struct font_t *font, const uint8_t *data);
uint8_t font_read_column(struct font_t *font, uint8_t charno, uint8_t column);
