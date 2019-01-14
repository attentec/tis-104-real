#include <avr/io.h>
#include <stdbool.h>

#include "panic.h"
#include "pin.h"

struct pin_t pin_init(enum port_t port, uint8_t bit, enum dir_t dir) {
    struct pin_t pin;
    pin.bit_pattern = 1 << bit;
    if (dir == PIN_DIR_INPUT) {
        switch (port) {
            case PIN_PORT_B:
                DDRB &= ~pin.bit_pattern;
                break;
            case PIN_PORT_C:
                DDRC &= ~pin.bit_pattern;
                break;
            case PIN_PORT_D:
                DDRD &= ~pin.bit_pattern;
                break;
            default:
                panic();
                break;
        }
    } else {
        switch (port) {
            case PIN_PORT_B:
                DDRB |= pin.bit_pattern;
                break;
            case PIN_PORT_C:
                DDRC |= pin.bit_pattern;
                break;
            case PIN_PORT_D:
                DDRD |= pin.bit_pattern;
                break;
            default:
                panic();
                break;
        }
    }
    switch (port) {
        case PIN_PORT_B:
            pin.port = &PORTB;
            pin.pin = &PINB;
            break;
        case PIN_PORT_C:
            pin.port = &PORTC;
            pin.pin = &PINC;
            break;
        case PIN_PORT_D:
            pin.port = &PORTD;
            pin.pin = &PIND;
            break;
        default:
            panic();
            break;
    }
    return pin;
}

void pin_write(struct pin_t *pin, bool value) {
    if (value) {
        *(pin->port) |= pin->bit_pattern;
    } else {
        *(pin->port) &= ~pin->bit_pattern;
    }
}

bool pin_read(struct pin_t *pin) {
    return (*(pin->pin) & pin->bit_pattern) != 0;
}
