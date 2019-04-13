#pragma once

#include <stdint.h>

struct surface_t;

uint8_t surface_get_width(struct surface_t *surface);
uint8_t surface_get_height(struct surface_t *surface);
void surface_set_pixel(struct surface_t *surface, uint8_t x, uint8_t y, uint16_t color);
