#pragma once

struct pin_t;

struct spi_t {
    struct pin_t *sdi;
    struct pin_t *clk;
};

void spi_sw_init(struct spi_t *spi, struct pin_t *sdi, struct pin_t *clk);
