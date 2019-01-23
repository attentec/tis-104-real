#include <avr/io.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "uart.h"
#include "uart_hw.h"

static uint16_t receive_bit(void) {
    while (PIND & (1 << 3));
    uint16_t result = (PIND & (1 << 2)) ? 1 : 0;
    while (!(PIND & (1 << 3)));
    return result;
}

static int16_t receive_word(void) {
    uint16_t bits = 0;
    if (receive_bit()) {
        bits = 0xffff;
    }
    for (uint8_t i = 0; i < 10; ++i) {
        bits = (bits << 1) | receive_bit();
    }
    return (int16_t) bits;
}

static bool try_receive_value(int16_t *value) {
    if (PIND & (1 << 3)) {
        // The 'write' pin was high, noone is sending to us
        return false;
    }

    // Set the 'read' pin low
    DDRD |= (1 << 2);
    // Wait for 'write' pin to become high
    while (!(PIND & (1 << 3)));
    // Set the 'read' pin high
    DDRD &= ~(1 << 2);

    // TODO: Receive value!!
    *value = receive_word();

    return true;
}

int main() {
    struct uart_t uart;

    // Set pin 2 and 3 to input, without pullup
    DDRD &= ~((1 << 3) | (1 << 2));
    PORTD &= ~((1 << 3) | (1 << 2));

    uart_hw_init(&uart);

    int16_t value;
    const size_t mysize = 100;
    uint8_t mybuf[mysize];
    while (true) {
        uart_read_char(&uart);
        //1R
        while (!try_receive_value(&value)) {
            uart_read_char(&uart);
            uart_read_char(&uart);
        }
        snprintf((char *) mybuf, mysize, "Got value: %" PRId16, value);
        uart_write_line(&uart, mybuf);
        uart_read_char(&uart);
        //1W
    }
}
