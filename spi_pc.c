#include <stdio.h>

#include "spi.h"

void spi_init(struct spi_t *spi, pin_t sdi, pin_t clk) {
    spi->sdi = sdi;
    spi->clk = clk;
}

void spi_write(struct spi_t *spi, uint8_t byte) {
    (void)printf("spi_write: %02x\n", byte);
}
