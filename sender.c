#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "uart.h"

static void signal_value_available(void) {
    DDRD |= (1 << 3);
}

static void transmit_bit(uint16_t bit) {
    if (bit) {
        DDRD &= ~(1 << 2);
    } else {
        DDRD |= (1 << 2);
    }
    _delay_us(1);
    DDRD |= (1 << 3);
    _delay_us(1);
    DDRD &= ~(1 << 3);
    _delay_us(1);
}

static void transmit_word(int16_t value) {
    uint16_t bits = (uint16_t) value;
    for (int i = 0; i < 11; i++) {
        transmit_bit(bits & (1 << 10));
        bits <<= 1;
    }
}

static bool try_transmit_value(int16_t value) {
    if (PIND & (1 << 2)) {
	// The 'read' pin was high, noone will receive our transmission
	return false;
    }

    // Set the 'write' pin high
    DDRD &= ~(1 << 3);
    // Wait for 'read' pin to become high
    while (!(PIND & (1 << 2)));

    // Transmit value!!
    transmit_word(value);

    return true;
}

int main() {
    // Set pin 2 and 3 to input, without pullup
    DDRD &= ~((1 << 3) | (1 << 2));
    PORTD &= ~((1 << 3) | (1 << 2));

    uart_init();

    int16_t counter = -999;
    while (true) {
	read_char();
	//1R
	read_char();
	//1W
	signal_value_available();
	read_char();
	//2R
	while (!try_transmit_value(counter)) {
	    read_char();
	    read_char();
	}
        write_line("Sent value");
	read_char();
	//2W

	counter += 1;
    }
}
