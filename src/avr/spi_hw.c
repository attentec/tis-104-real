#include <avr/io.h>
#include <stdint.h>

#include "spi_hw.h"

void spi_init(struct spi_t *spi) {
    spi->dummy = 0;
    DDRB |= ((1<<5) | // SCK
             (1<<3) | // MOSI
             (1<<2)); // SS
    DDRB &= ~(1<<4);  // MISO
    PORTB |= (1<<4);  // MISO pullup
    SPCR = ((0<<SPIE) |
            (1<<SPE) |
            (0<<DORD) |
            (1<<MSTR) |
            (0<<CPOL) |
            (0<<CPHA) |
            (0<<SPR1) |
            (0<<SPR0));
    SPSR = (1<<SPI2X);
}

void spi_write(struct spi_t *spi, uint8_t byte) {
    (void) spi;
    SPDR = byte;
    while ((SPSR & (1<<SPIF)) == 0) {
    }
}
