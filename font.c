#include "font.h"
#include "mem.h"

void font_init(struct font *font, const uint8_t *data) {
    font->data = data;
    font->width = 6;
    font->height = 8;
}

uint8_t font_read_column(struct font *font, uint8_t charno, uint8_t column) {
    uint16_t char_offset = charno;
    uint16_t char_index = font->width * char_offset;
    return mem_read_byte(&font->data[char_index + column]);
}
