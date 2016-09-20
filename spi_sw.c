#include "spi.h"
#include "delay.h"

void spi_init(struct spi_t *spi, pin_t sdi, pin_t clk) {
    spi->sdi = sdi;
    spi->clk = clk;
}

void spi_write(struct spi_t *spi, uint8_t byte) {
    const uint8_t delay = 1;
    delay_us(delay);
    for (int i = 0; i < 8; ++i) {
        if (byte & (1 << 7)) {
            pin_write(spi->sdi, true);
        } else {
            pin_write(spi->sdi, false);
        }
        delay_us(delay);
        pin_write(spi->clk, true);
        delay_us(delay);
        pin_write(spi->clk, false);
        byte <<= 1;
    }
    delay_us(delay);
}
