#include <stdint.h>

#include "display.h"
#include "platform.h"
#include "runner.h"

int main(void) 
{
    runner_main(ORIENTATION_RIBBON_BOTTOM);
    return 0;
}

static uint8_t w;
static uint8_t h;
static uint16_t white = RGB888_TO_RGB565(0xFFFFFFul);
static uint16_t black = RGB888_TO_RGB565(0x000000ul);

void app_init(void)
{
    display_set_write_order(platform.display, WRITE_ORDER_X_MAJOR);
    w = display_get_width(platform.display);
    h = display_get_height(platform.display);
    display_set_window(platform.display, 0, 0, w, h);
}

bool app_loop(void)
{
    for (uint8_t y = 0; y < h; y++) {
        for (uint8_t x = 0; x < w; x++) {
            display_write_pixel(platform.display, white);
        }
    }
    for (uint8_t y = 0; y < h; y++) {
        for (uint8_t x = 0; x < w; x++) {
            display_write_pixel(platform.display, black);
        }
    }
    return true;
}

