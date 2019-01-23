#include <stdbool.h>
#include <stdint.h>

#include "pin.h"
#include "spi.h"
#include "spi_sw.h"

void spi_sw_init(struct spi_t *spi, struct pin_t *sdi, struct pin_t *clk) {
    spi->sdi = sdi;
    spi->clk = clk;
}

void spi_write(struct spi_t *spi, uint16_t word) {
    for (uint8_t i = 0; i < 16; ++i) {
        if (word & (1 << 15)) {
            pin_write(spi->sdi, true);
        } else {
            pin_write(spi->sdi, false);
        }
        pin_write(spi->clk, true);
        pin_write(spi->clk, false);
        word <<= 1;
    }
}
