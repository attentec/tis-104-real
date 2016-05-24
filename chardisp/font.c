#ifdef __AVR__
#include <avr/pgmspace.h>
#endif
#include "font.h"

FontInfo font_make_fontinfo(Font *font) {
    FontInfo info = {
        .font = font,
        .width = pgm_read_byte(&font[0]),
        .height = pgm_read_byte(&font[1]),
        .offset = pgm_read_byte(&font[2]),
        .numchars = pgm_read_byte(&font[3]),
    };
    return info;
}

int font_char_index(FontInfo *font, int charno) {
	int char_offset = font->width + 1;  // bytes used by each character
	return (char_offset * charno) + FONT_HEADER_SIZE;  // char offset (add 4 for font header)
}

int font_read_byte(FontInfo *font, int index) {
    return pgm_read_byte(&font->font[index]);
}

void font_sprint_char(FontInfo *font, char *buf, char ch, int line) {
	uint8_t char_data, char_width;
	uint8_t i, j;
	uint16_t char_offset;

	char_offset = font_char_index(font, ch);
	char_offset++;  // increment pointer to first character data byte

	for (i = 0; i < font->width; i++) {  // each font "column"
        char_data = font_read_byte(font, char_offset);
        char_offset++;

        if (line >= font->height) {
            break;  // No need to process excess bits
        }
        if (char_data & (1 << line)) {
            buf[i] = '#';
        } else {
            buf[i] = ' ';
        }
	}
}
