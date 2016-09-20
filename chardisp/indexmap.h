#pragma once

#include <stdint.h>
#include <stdbool.h>

struct indexmap;

void indexmap_init(struct indexmap *map, uint8_t width, uint8_t height, uint8_t *buf);
uint8_t indexmap_width(struct indexmap *map);
uint8_t indexmap_height(struct indexmap *map);
void indexmap_set(struct indexmap *map, uint8_t x, uint8_t y, uint8_t val);
uint8_t indexmap_get(struct indexmap *map, uint8_t x, uint8_t y);
bool indexmap_is_dirty(struct indexmap *map, uint8_t x, uint8_t y);
void indexmap_clear_dirty(struct indexmap *map, uint8_t x, uint8_t y);
void indexmap_print(struct indexmap *map);
