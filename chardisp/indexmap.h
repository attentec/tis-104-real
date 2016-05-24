#pragma once

struct IndexMap;
typedef struct IndexMap* indexmap;

indexmap indexmap_init(int width, int height);
int indexmap_width(indexmap map);
int indexmap_height(indexmap map);
void indexmap_set(indexmap map, int x, int y, int val);
int indexmap_get(indexmap map, int x, int y);
void indexmap_print(indexmap grid);
