#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t pin_t;

enum dir_t {
    PIN_DIR_INPUT  = 0,
    PIN_DIR_OUTPUT = 1,
};

enum port_t {
    PIN_PORT_B = 1,
    PIN_PORT_C = 2,
    PIN_PORT_D = 3,
};

pin_t pin_init(enum port_t port, uint8_t bit, enum dir_t dir);
void pin_write(pin_t pin, bool value);
bool pin_read(pin_t pin);
