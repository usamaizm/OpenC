#include <stdio.h>
#include "render.h"

#define ANSI_CLEAR_SCREEN "\033[2J"
#define ANSI_CURSOR_HOME "\033[H"
#define ANSI_CURSOR_HIDE "\033[?25l"
#define ANSI_CURSOR_SHOW "\033[?25h"

void clear_screen(void) {
    printf("%s%s", ANSI_CLEAR_SCREEN, ANSI_CURSOR_HOME);
}

void render_move_cursor(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1);
}

void render_text_buffer(const TextBuffer *buffer) {
    if (!buffer) return;
    printf("%s%s%s", ANSI_CURSOR_HIDE, ANSI_CLEAR_SCREEN, ANSI_CURSOR_HOME);
    for (size_t i = 0; i < buffer->line_count; ++i) {
        printf("%s\r\n", buffer->lines[i]);
    }
    render_move_cursor((int)buffer->cursor_row, (int)buffer->cursor_col);
    printf("%s", ANSI_CURSOR_SHOW);
    fflush(stdout);
}
