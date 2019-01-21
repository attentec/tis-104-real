#include "icon.h"
#include "mem.h"

bool icon_read_pixel(struct icon_t *icon, uint8_t x, uint8_t y)
{
    uint8_t y_major = y / 8;
    uint8_t y_minor = y % 8;
    uint8_t col_pixels = mem_read_byte(&icon->data[(icon->width * y_major) + x]);
    return (col_pixels >> y_minor) & 0x01;
}
