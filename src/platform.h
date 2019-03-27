#pragma once

#include <stdbool.h>

struct display_t;
struct pin_t;
struct uart_t;

enum orientation_t {
    ORIENTATION_RIBBON_BOTTOM,
    ORIENTATION_RIBBON_LEFT,
    ORIENTATION_RIBBON_TOP,
    ORIENTATION_RIBBON_RIGHT,
    ORIENTATION_DEFAULT = ORIENTATION_RIBBON_LEFT
};

struct platform_t {
    struct display_t *display;
    struct pin_t *led;
    struct uart_t *uart;
};

extern struct platform_t platform;

void platform_init(enum orientation_t orientation);
void platform_begin(void);
bool platform_loop(void);
void platform_deinit(void);
