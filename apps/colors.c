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
            uint8_t red = ((uint16_t) x * 255u) / 175u;
            uint8_t green = ((uint16_t) y * 255u) / 219u;
            uint8_t blue = 0;
            uint32_t color = 0;
            color |= red;
            color <<= 8u;
            color |= green;
            color <<= 8u;
            color |= blue;
            display_write_pixel(platform.display, RGB888_TO_RGB565(color));
        }
    }
    for (uint8_t y = 0; y < h; y++) {
        for (uint8_t x = 0; x < w; x++) {
            uint8_t red = ((uint16_t) y * 255u) / 219u;
            uint8_t green = 255;
            uint8_t blue = ((uint16_t) x * 255u) / 175u;
            uint32_t color = 0;
            color |= red;
            color <<= 8u;
            color |= green;
            color <<= 8u;
            color |= blue;
            display_write_pixel(platform.display, RGB888_TO_RGB565(color));
        }
    }
    return true;
}
