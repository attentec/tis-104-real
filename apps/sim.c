#include "canvas.h"
#include "code.h"
#include "display.h"
#include "fonts.h"
#include "icons.h"
#include "platform.h"
#include "runner.h"
#include "tile.h"

const source_t example_program_text = {
    "  MOV 10, ACC",
    "LOOP:",
    "  SUB 1",
    "  JNZ LOOP",
    "  SWP",
    "  ADD 1",
    "  SWP",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

int main(int argc, char **argv)
{
    runner_main(ORIENTATION_RIBBON_LEFT, 1, 1, argc, argv);
    return 0;
}

static struct canvas_t canvas;
static struct code_t code;
static struct tile_t tile;

void app_init(void)
{
    canvas_init(&canvas, platform.display, &monoblipp6x8);
    code_init(&code, &example_program_text);
    tile_init(&tile, &code, &canvas);
}

bool app_loop(void)
{
    tile_draw(&tile);
    tile_read(&tile);
    tile_write(&tile);
    return true;
}
