#pragma once

#include <stdbool.h>
#include <stdint.h>

struct icon_t
{
    const uint8_t* data;
    uint8_t width;
    uint8_t height;
};

bool icon_read_pixel(struct icon_t *icon, uint8_t x, uint8_t y);
