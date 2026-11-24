#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H

#include <stddef.h>

typedef struct {
    char **lines;
    size_t line_count;
    size_t capacity;
    size_t cursor_row;
    size_t cursor_col;
} TextBuffer;

TextBuffer *init_text_buffer(void);
void free_text_buffer(TextBuffer *buffer);
int append_line(TextBuffer *buffer, const char *line);
int insert_character(TextBuffer *buffer, char c);
int insert_newline(TextBuffer *buffer);
int delete_character(TextBuffer *buffer);
void move_cursor(TextBuffer *buffer, int row_change, int col_change);

#endif
