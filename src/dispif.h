#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "pin.h"

struct spi_t;

struct dispif_t {
    struct pin_t *rs;
    struct pin_t *cs;
    struct pin_t *rst;
    struct pin_t *led;
    struct spi_t *spi;
};

void dispif_init(struct dispif_t *dispif, struct spi_t *spi, struct pin_t *rs, struct pin_t *cs, struct pin_t *rst, struct pin_t *led);
void dispif_write_data(struct dispif_t *dispif, uint16_t data);
void dispif_write_command(struct dispif_t *dispif, uint16_t command);
void dispif_write_register(struct dispif_t *dispif, uint16_t command, uint16_t data);
void dispif_set_backlight(struct dispif_t *dispif, bool on);
void dispif_set_reset(struct dispif_t *dispif, bool on);
