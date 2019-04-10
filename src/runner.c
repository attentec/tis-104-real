#include "app.h"
#include "delay.h"
#include "platform.h"
#include "runner.h"

static void runner_main_common(void);

void runner_main(enum orientation_t orientation)
{
    platform_init(orientation);
    runner_main_common();
}

void runner_main_multi(enum orientation_t orientation, int cols, int rows)
{
    platform_init_multi(orientation, cols, rows);
    runner_main_common();
}

static void runner_main_common(void)
{
    app_init();
    platform_begin();
    for (;;) {
        if (!platform_loop()) {
            break;
        }
        if (!app_loop()) {
            break;
        }
        delay_ms(500);
    }
    platform_deinit();
}
