#include <stdio.h>

#include "spi.h"
#include "spi_stdio.h"

void spi_stdio_init(struct spi_t *spi)
{
    (void) spi;
}

void spi_write(struct spi_t *spi, uint16_t word)
{
    (void) spi;
    (void) printf("spi_write: %04x\n", word);
}
