#pragma once

#include <inttypes.h>

#define FONT_HEADER_SIZE 4 // 1: pixel width of 1 font character, 2: pixel height, 

typedef const uint8_t Font;

typedef struct
{
	uint8_t* font;
	uint8_t width;
	uint8_t height;
	uint8_t offset;
	uint8_t numchars;
} FontInfo;

FontInfo font_make_fontinfo(Font *font);
int font_char_index(FontInfo *font, int charno);
int font_read_byte(FontInfo *font, int index);
void font_sprint_char(FontInfo *font, char *buf, char ch, int line);
