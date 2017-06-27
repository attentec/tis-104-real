#include <avr/io.h>

#include "uart.h"

static void uart_9600(void) {
#define BAUD 9600
#include <util/setbaud.h>
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif
}

void uart_init(void) {
    uart_9600();

    UCSR0A = 0;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02);
    UCSR0C = ((0 << UMSEL01) | (0 << UMSEL00) |
              (0 << UPM01) | (0 << UPM00) |
              (0 << USBS0) |
              (1 << UCSZ01) | (1 << UCSZ00) |
              (0 << UCPOL0));
}

void write_char(uint8_t c) {
    UDR0 = c;
    while (!(UCSR0A & (1 << TXC0)));
    UCSR0A |= (1 << TXC0);
}

uint8_t read_char(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void write_line(uint8_t *s) {
    do {
        write_char(*s);
    } while (*++s);
    write_char('\r');
    write_char('\n');
}
