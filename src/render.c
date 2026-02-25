#include <stdio.h>
#include "render.h"

// ANSI escape codes
#define ANSI_CLEAR_SCREEN "\033[2J"
#define ANSI_CURSOR_HOME "\033[H"
#define ANSI_CURSOR_HIDE "\033[?25l"
#define ANSI_CURSOR_SHOW "\033[?25h"

// Clear the terminal screen
void clear_screen(void) {
    printf("%s%s", ANSI_CLEAR_SCREEN, ANSI_CURSOR_HOME);
    fflush(stdout);
}

// Move cursor to a specific row and column
void move_cursor(int row, int col) {
    printf("\033[%d;%dH", row + 1, col + 1); // ANSI is 1-based
    fflush(stdout);
}

// Render the text buffer to the terminal
void render_text_buffer(const TextBuffer *buffer) {
    clear_screen();
    printf("%s", ANSI_CURSOR_HIDE);

    for (int i = 0; i < buffer->num_lines; i++) {
        printf("%s\n", buffer->lines[i]);
    }

    // Move cursor to its position in the buffer
    move_cursor(buffer->cursor_row, buffer->cursor_col);
    printf("%s", ANSI_CURSOR_SHOW);
    fflush(stdout);
}