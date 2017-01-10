#include "spi.h"
#include "spi_sw.h"

void spi_init(struct spi_t *spi, pin_t sdi, pin_t clk) {
    spi->sdi = sdi;
    spi->clk = clk;
}

void spi_write(struct spi_t *spi, uint8_t byte) {
    for (uint8_t i = 0; i < 8; ++i) {
        if (byte & (1 << 7)) {
            pin_write(spi->sdi, true);
        } else {
            pin_write(spi->sdi, false);
        }
        pin_write(spi->clk, true);
        pin_write(spi->clk, false);
        byte <<= 1;
    }
}
