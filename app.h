#pragma once

#include <stdbool.h>

struct screen;
struct tft_t;

void app_init(struct screen *screen, struct tft_t *tft);
bool app_loop(struct screen *scr, struct tft_t *tft);
void app_deinit(struct tft_t *tft);
