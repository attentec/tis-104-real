#include "board.h"
#include "canvas.h"
#include "code.h"
#include "display.h"
#include "dispif.h"
#include "fonts.h"
#include "icons.h"
#include "tile.h"

const char *example_program_text[CPU_MAX_PRGM_LENGTH] = {
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

int main(void)
{
    struct display_t display;
    struct canvas_t canvas;
    struct code_t code;
    struct tile_t tile;

    board_init();
    display_init(&display, board.dispif, ORIENTATION_RIBBON_LEFT, WRITE_ORDER_Y_MAJOR);
    canvas_init(&canvas, &display, &monoblipp6x8);
    code_init(&code, example_program_text);
    tile_init(&tile, &code, &canvas);
    display_activate(&display);

    for (;;) {
        tile_draw(&tile);
        tile_read(&tile);
        tile_write(&tile);
    }

    return 0;
}
