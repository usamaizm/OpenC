#include "text_buffer.h"

#include <assert.h>
#include <string.h>

int main(void) {
    TextBuffer *buffer = init_text_buffer();
    assert(buffer != NULL);
    assert(buffer->line_count == 1);
    assert(append_line(buffer, "second") == 0);
    assert(buffer->line_count == 2);

    assert(insert_character(buffer, 'a') == 0);
    assert(insert_character(buffer, 'b') == 0);
    assert(strcmp(buffer->lines[0], "ab") == 0);

    assert(insert_newline(buffer) == 0);
    assert(buffer->line_count == 3);
    assert(strcmp(buffer->lines[0], "ab") == 0);
    assert(strcmp(buffer->lines[1], "") == 0);
    assert(strcmp(buffer->lines[2], "second") == 0);

    assert(insert_character(buffer, 'x') == 0);
    assert(delete_character(buffer) == 0);
    assert(strcmp(buffer->lines[1], "") == 0);
    assert(delete_character(buffer) == 0);
    assert(buffer->line_count == 2);
    assert(strcmp(buffer->lines[0], "ab") == 0);
    assert(strcmp(buffer->lines[1], "second") == 0);

    move_cursor(buffer, 1, 100);
    assert(buffer->cursor_row == 1);
    assert(buffer->cursor_col == strlen("second"));
    move_cursor(buffer, -100, -100);
    assert(buffer->cursor_row == 0);
    assert(buffer->cursor_col == 0);

    free_text_buffer(buffer);
    return 0;
}
