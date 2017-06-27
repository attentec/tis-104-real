#include <stdbool.h>
#include <stdint.h>

#include "pin.h"

pin_t pin_init(enum port_t port, uint8_t bit, enum dir_t dir) {
    (void)port;
    (void)bit;
    (void)dir;
    return 0;
}

void pin_write(pin_t pin, bool value) {
    (void)pin;
    (void)value;
}

bool pin_read(pin_t pin) {
    (void)pin;
    return false;
}
