#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

int main() {
    DDRB = 1 << 5;

    while (true) {
        _delay_ms(500);
        PINB |= 1 << 5;
    }

    return 0;
}
