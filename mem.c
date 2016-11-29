#include "mem.h"

#ifdef __AVR__
#include <avr/pgmspace.h>

uint8_t mem_read_byte(const void *ptr) {
    return pgm_read_byte(ptr);
}

#else

uint8_t mem_read_byte(const void *ptr) {
    return *((uint8_t *) ptr);
}

#endif
