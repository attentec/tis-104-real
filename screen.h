#pragma once
#include "indexmap.h"
#include "font.h"

struct Screen;
typedef struct Screen* screen;

screen screen_init(struct indexmap *indices, struct font *font);
void screen_set(screen scr, int x, int y, int val);
int screen_get(screen scr, int x, int y);
void screen_enable_cursor(screen scr);
void screen_disable_cursor(screen scr);
void screen_move_cursor(screen scr, int x, int y);
void screen_print(screen scr);
void screen_print_dirty(screen scr);

typedef struct {
    screen scr;
    int index;
    int x;
    int y;
} DirtyIterator;
void screen_get_dirties(screen scr, DirtyIterator *dirties);
int screen_get_next_dirty(DirtyIterator *dirties);
