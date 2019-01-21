#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif
#include "icon.h"
#include "icons.h"

const uint8_t arrow_icon_data[] PROGMEM = {
    0x00, 0x00, 0xfe, 0x02, 0x02, 0x02, 0xfe, 0x00, 0x00,
    0x08, 0x18, 0x2f, 0x40, 0x80, 0x40, 0x2f, 0x18, 0x08
};

struct icon_t arrow_icon = {
    .data = arrow_icon_data,
    .width = 9,
    .height = 16
};
