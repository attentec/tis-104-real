#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "indexmap.h"

struct IndexMap {
    int width;
    int height;
    int indices[];
};

indexmap indexmap_init(int width, int height) {
    const size_t size = width * height;
    indexmap map = malloc(sizeof(struct IndexMap) + size * sizeof(int));
    memset(map, 0, sizeof(struct IndexMap) + size * sizeof(int));
    map->width = width;
    map->height = height;
    return map;
}

void indexmap_set(indexmap map, int x, int y, int val) {
    map->indices[y * map->width + x] = val;
}

int indexmap_get(indexmap map, int x, int y) {
    return map->indices[y * map->width + x];
}

int indexmap_width(indexmap map) {
    return map->width;
}

int indexmap_height(indexmap map) {
    return map->height;
}

void indexmap_print(indexmap map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            printf("%d", map->indices[i * map->width + j]);
        }
        puts("");
    }
}
