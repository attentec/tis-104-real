#include "app.h"
#include "delay.h"
#include "platform.h"
#include "runner.h"

static void runner_main_common(void);

void runner_main(enum orientation_t orientation, int cols, int rows, int argc, char **argv)
{
    platform_init(orientation, cols, rows, argc, argv);
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
    }
    platform_deinit();
}
