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

#define COLS (4)
#define ROWS (3)

int main(int argc, char **argv)
{
    runner_main(ORIENTATION_RIBBON_LEFT, COLS, ROWS, argc, argv);
    return 0;
}

static struct canvas_t canvases[COLS][ROWS];
static struct code_t codes[COLS][ROWS];
static struct tile_t tiles[COLS][ROWS];

void app_init(void)
{
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS; r++) {
            struct display_t *display = platform_get_display(c, r);
            canvas_init(&canvases[c][r], display, &monoblipp6x8);
            code_init(&codes[c][r], &example_program_text);
            tile_init(&tiles[c][r], &codes[c][r], &canvases[c][r]);
        }
    }
}

bool app_loop(void)
{
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS; r++) {
            tile_draw(&tiles[c][r]);
        }
    }
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS; r++) {
            tile_read(&tiles[c][r]);
        }
    }
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS; r++) {
            tile_write(&tiles[c][r]);
        }
    }
    return true;
}
