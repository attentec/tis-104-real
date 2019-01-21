#include <stdint.h>
#include <stdio.h>

struct spi_t;

void spi_write(struct spi_t *spi, uint16_t word) {
    (void)spi;
    (void)printf("spi_write: %04x\n", word);
}
