#pragma once

#include <stdbool.h>
#include <stdint.h>

struct indexmap_t {
    uint8_t width;
    uint8_t height;
    uint8_t *indices;
};

void indexmap_init(struct indexmap_t *map, uint8_t width, uint8_t height, uint8_t *buf);
uint8_t indexmap_width(struct indexmap_t *map);
uint8_t indexmap_height(struct indexmap_t *map);
void indexmap_set(struct indexmap_t *map, uint8_t x, uint8_t y, uint8_t val);
uint8_t indexmap_get(struct indexmap_t *map, uint8_t x, uint8_t y);
bool indexmap_is_dirty(struct indexmap_t *map, uint8_t x, uint8_t y);
void indexmap_set_dirty(struct indexmap_t *map, uint8_t x, uint8_t y);
void indexmap_clear_dirty(struct indexmap_t *map, uint8_t x, uint8_t y);
void indexmap_print(struct indexmap_t *map);
