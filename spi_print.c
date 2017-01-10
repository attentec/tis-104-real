#include <stdio.h>

#include "spi.h"

void spi_write(struct spi_t *spi, uint8_t byte) {
    (void)spi;
    (void)printf("spi_write: %02x\n", byte);
}
