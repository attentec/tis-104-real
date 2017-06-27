#include <avr/io.h>
#include <stdbool.h>

#include "panic.h"
#include "pin.h"

#define PORT_MASK (0x0Fu)
#define PIN_MASK  (0x0Fu)
#define PIN_BITS  (4u)

static pin_t make_pin(enum port_t port, uint8_t bit) {
    return (port << PIN_BITS) | (bit & PIN_MASK);
}

static enum port_t get_port(pin_t pin) {
    return (pin >> PIN_BITS) & PORT_MASK;
}

pin_t pin_init(enum port_t port, uint8_t bit, enum dir_t dir) {
    uint8_t bit_pattern = 1 << bit;
    if (dir == PIN_DIR_INPUT) {
        switch (port) {
            case PIN_PORT_B:
                DDRB &= ~bit_pattern;
                break;
            case PIN_PORT_C:
                DDRC &= ~bit_pattern;
                break;
            case PIN_PORT_D:
                DDRD &= ~bit_pattern;
                break;
            default:
                panic();
                break;
        }
    } else {
        switch (port) {
            case PIN_PORT_B:
                DDRB |= bit_pattern;
                break;
            case PIN_PORT_C:
                DDRC |= bit_pattern;
                break;
            case PIN_PORT_D:
                DDRD |= bit_pattern;
                break;
            default:
                panic();
                break;
        }
    }
    return make_pin(port, bit);
}

void pin_write(pin_t pin, bool value) {
    enum port_t port = get_port(pin);
    uint8_t bit = pin & PIN_MASK;
    uint8_t bit_pattern = 1 << bit;
    if (value) {
        switch (port) {
            case PIN_PORT_B:
                PORTB |= bit_pattern;
                break;
            case PIN_PORT_C:
                PORTC |= bit_pattern;
                break;
            case PIN_PORT_D:
                PORTD |= bit_pattern;
                break;
            default:
                panic();
                break;
        }
    } else {
        switch (port) {
            case PIN_PORT_B:
                PORTB &= ~bit_pattern;
                break;
            case PIN_PORT_C:
                PORTC &= ~bit_pattern;
                break;
            case PIN_PORT_D:
                PORTD &= ~bit_pattern;
                break;
            default:
                panic();
                break;
        }
    }
}

bool pin_read(pin_t pin) {
    enum port_t port = get_port(pin);
    uint8_t bit = pin & PIN_MASK;
    uint8_t bit_pattern = 1 << bit;
    switch (port) {
        case PIN_PORT_B:
            return (PINB & bit_pattern) != 0;
        case PIN_PORT_C:
            return (PINC & bit_pattern) != 0;
        case PIN_PORT_D:
            return (PIND & bit_pattern) != 0;
        default:
            panic();
            return false;
    }
}
