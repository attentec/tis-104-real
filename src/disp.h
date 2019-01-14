#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "pin.h"

struct spi_t;

struct disp_t {
    struct pin_t *rs;
    struct pin_t *cs;
    struct pin_t *rst;
    struct pin_t *led;
    struct spi_t *spi;
};

void disp_init(struct disp_t *disp, struct spi_t *spi, struct pin_t *rs, struct pin_t *cs, struct pin_t *rst, struct pin_t *led);
void disp_write_data(struct disp_t *disp, uint16_t data);
void disp_write_command(struct disp_t *disp, uint16_t command);
void disp_write_register(struct disp_t *disp, uint16_t command, uint16_t data);
void disp_set_backlight(struct disp_t *disp, bool on);
void disp_set_reset(struct disp_t *disp, bool on);
