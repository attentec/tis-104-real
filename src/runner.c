#include "app.h"
#include "platform.h"
#include "runner.h"

void runner_main(enum orientation_t orientation)
{
    platform_init(orientation);
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
