#include <stdbool.h>
#include <stdint.h>

#include "pin.h"

struct pin_t pin_init(enum port_t port, uint8_t bit, enum dir_t dir) {
    struct pin_t pin;
    pin.port = 0;
    pin.pin = 0;
    pin.bit_pattern = 0;
    (void)port;
    (void)bit;
    (void)dir;
    return pin;
}

void pin_write(struct pin_t *pin, bool value) {
    (void)pin;
    (void)value;
}

bool pin_read(struct pin_t *pin) {
    (void)pin;
    return false;
}
