#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "indexmap.h"

#define DIRTY_MASK (0x80u)

struct indexmap {
    uint8_t width;
    uint8_t height;
    uint8_t *indices;
};

void indexmap_init(struct indexmap *map, uint8_t width, uint8_t height, uint8_t *buf) {
    map->width = width;
    map->height = height;
    map->indices = buf;
}

static inline uint16_t index(struct indexmap *map, uint8_t x, uint8_t y) {
    uint16_t w16 = map->width;
    uint16_t x16 = x;
    uint16_t y16 = y;
    return y16 * w16 + x16;
}

void indexmap_set(struct indexmap *map, uint8_t x, uint8_t y, uint8_t val) {
    uint8_t new_val = val | DIRTY_MASK;
    map->indices[index(map, x, y)] = new_val;
}

uint8_t indexmap_get(struct indexmap *map, uint8_t x, uint8_t y) {
    uint8_t val = map->indices[index(map, x, y)];
    return val & ~DIRTY_MASK;
}

bool indexmap_is_dirty(struct indexmap *map, uint8_t x, uint8_t y) {
    uint8_t val = map->indices[index(map, x, y)];
    return (val & DIRTY_MASK) != 0u;
}

void indexmap_clear_dirty(struct indexmap *map, uint8_t x, uint8_t y) {
    map->indices[index(map, x, y)] &= ~DIRTY_MASK;
}

uint8_t indexmap_width(struct indexmap *map) {
    return map->width;
}

uint8_t indexmap_height(struct indexmap *map) {
    return map->height;
}

void indexmap_print(struct indexmap *map) {
    for (uint8_t y = 0; y < map->height; ++y) {
        for (uint8_t x = 0; x < map->width; ++x) {
            printf("%02x ", indexmap_get(map, x, y));
        }
        puts("");
    }
}
