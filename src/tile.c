#include "canvas.h"
#include "code.h"
#include "cpu.h"
#include "gui.h"
#include "tile.h"

void tile_init(struct tile_t *tile, struct code_t *code, struct canvas_t *canvas)
{
    tile->code = *code;
    tile->canvas = canvas;
    cpu_state_init(&tile->cpu_state);
    cpu_init(&tile->cpu, &code->prgm, &tile->cpu_state);
    tile->last_line = code->addr_to_line[0];
    gui_show_cpu(canvas, code->lines);
}

void tile_draw(struct tile_t *tile)
{
    uint8_t current_line = tile->code.addr_to_line[tile->cpu_state.pc];
    gui_update_cpu(tile->canvas, tile->code.lines, &tile->cpu_state, tile->last_line, current_line);
    tile->last_line = current_line;
}

void tile_read(struct tile_t *tile)
{
    cpu_read(&tile->cpu);
}

void tile_write(struct tile_t *tile)
{
    cpu_write(&tile->cpu);
}
