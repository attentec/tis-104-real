#include "indexmap.h"
#include "screen.h"

int pgm_read_byte(int *p) {
    return *p;
}

extern Font Terminal6x8[];

int main() {
    indexmap indices = indexmap_init(3, 3);
    FontInfo font = font_make_fontinfo(Terminal6x8);
    screen scr = screen_init(indices, &font);
    screen_set(scr, 0, 0, 4);
    screen_set(scr, 1, 0, 10);
    screen_set(scr, 2, 0, 3);
    screen_set(scr, 0, 1, 8);
    screen_set(scr, 1, 1, 11);
    screen_set(scr, 2, 1, 7);
    screen_set(scr, 0, 2, 6);
    screen_set(scr, 1, 2, 9);
    screen_set(scr, 2, 2, 5);
    DirtyIterator dirties;
    screen_get_dirties(scr, &dirties);
    while (screen_get_next_dirty(&dirties)) {
        screen_get(scr, dirties.x, dirties.y);
    }
    screen_move_cursor(scr, 1, 1);
    screen_enable_cursor(scr);
    screen_print_dirty(scr);
    return 0;
}
