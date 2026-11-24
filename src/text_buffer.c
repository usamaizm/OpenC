#include "text_buffer.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int reserve_lines(TextBuffer *buffer, size_t needed) {
    if (!buffer) return -1;
    if (needed <= buffer->capacity) return 0;
    size_t next = buffer->capacity ? buffer->capacity : 8;
    while (next < needed) {
        if (next > SIZE_MAX / 2) return -1;
        next *= 2;
    }
    if (next > SIZE_MAX / sizeof(*buffer->lines)) return -1;
    char **lines = realloc(buffer->lines, next * sizeof(*lines));
    if (!lines) return -1;
    buffer->lines = lines;
    buffer->capacity = next;
    return 0;
}

TextBuffer *init_text_buffer(void) {
    TextBuffer *buffer = calloc(1, sizeof(*buffer));
    if (!buffer) return NULL;
    if (reserve_lines(buffer, 1) != 0) {
        free(buffer);
        return NULL;
    }
    buffer->lines[0] = calloc(1, 1);
    if (!buffer->lines[0]) {
        free(buffer->lines);
        free(buffer);
        return NULL;
    }
    buffer->line_count = 1;
    return buffer;
}

void free_text_buffer(TextBuffer *buffer) {
    if (!buffer) return;
    for (size_t i = 0; i < buffer->line_count; ++i) free(buffer->lines[i]);
    free(buffer->lines);
    free(buffer);
}

int append_line(TextBuffer *buffer, const char *line) {
    if (!buffer || !line || reserve_lines(buffer, buffer->line_count + 1) != 0) return -1;
    size_t length = strlen(line);
    char *copy = malloc(length + 1);
    if (!copy) return -1;
    memcpy(copy, line, length + 1);
    buffer->lines[buffer->line_count++] = copy;
    return 0;
}

int insert_character(TextBuffer *buffer, char c) {
    if (!buffer || buffer->cursor_row >= buffer->line_count) return -1;
    char *line = buffer->lines[buffer->cursor_row];
    size_t length = strlen(line);
    if (buffer->cursor_col > length || length == SIZE_MAX) return -1;
    char *grown = realloc(line, length + 2);
    if (!grown) return -1;
    memmove(grown + buffer->cursor_col + 1, grown + buffer->cursor_col,
            length - buffer->cursor_col + 1);
    grown[buffer->cursor_col] = c;
    buffer->lines[buffer->cursor_row] = grown;
    ++buffer->cursor_col;
    return 0;
}

int insert_newline(TextBuffer *buffer) {
    if (!buffer || buffer->cursor_row >= buffer->line_count) return -1;
    if (reserve_lines(buffer, buffer->line_count + 1) != 0) return -1;
    char *line = buffer->lines[buffer->cursor_row];
    size_t length = strlen(line);
    if (buffer->cursor_col > length) return -1;
    char *tail = malloc(length - buffer->cursor_col + 1);
    if (!tail) return -1;
    memcpy(tail, line + buffer->cursor_col, length - buffer->cursor_col + 1);
    line[buffer->cursor_col] = '\0';
    memmove(buffer->lines + buffer->cursor_row + 2,
            buffer->lines + buffer->cursor_row + 1,
            (buffer->line_count - buffer->cursor_row - 1) * sizeof(*buffer->lines));
    buffer->lines[buffer->cursor_row + 1] = tail;
    ++buffer->line_count;
    ++buffer->cursor_row;
    buffer->cursor_col = 0;
    return 0;
}

int delete_character(TextBuffer *buffer) {
    if (!buffer || buffer->cursor_row >= buffer->line_count) return -1;
    char *line = buffer->lines[buffer->cursor_row];
    size_t length = strlen(line);
    if (buffer->cursor_col > length) return -1;
    if (buffer->cursor_col > 0) {
        memmove(line + buffer->cursor_col - 1, line + buffer->cursor_col,
                length - buffer->cursor_col + 1);
        --buffer->cursor_col;
        return 0;
    }
    if (buffer->cursor_row == 0) return 0;
    char *previous = buffer->lines[buffer->cursor_row - 1];
    size_t previous_length = strlen(previous);
    if (length > SIZE_MAX - previous_length - 1) return -1;
    char *joined = realloc(previous, previous_length + length + 1);
    if (!joined) return -1;
    memcpy(joined + previous_length, line, length + 1);
    buffer->lines[buffer->cursor_row - 1] = joined;
    free(line);
    memmove(buffer->lines + buffer->cursor_row,
            buffer->lines + buffer->cursor_row + 1,
            (buffer->line_count - buffer->cursor_row - 1) * sizeof(*buffer->lines));
    --buffer->line_count;
    --buffer->cursor_row;
    buffer->cursor_col = previous_length;
    return 0;
}

void move_cursor(TextBuffer *buffer, int row_change, int col_change) {
    if (!buffer || buffer->line_count == 0) return;
    if (row_change < 0) {
        size_t amount = (size_t)(-(row_change + 1)) + 1;
        buffer->cursor_row = amount > buffer->cursor_row ? 0 : buffer->cursor_row - amount;
    } else {
        size_t amount = (size_t)row_change;
        size_t last = buffer->line_count - 1;
        buffer->cursor_row = amount > last - buffer->cursor_row ? last : buffer->cursor_row + amount;
    }
    size_t length = strlen(buffer->lines[buffer->cursor_row]);
    if (col_change < 0) {
        size_t amount = (size_t)(-(col_change + 1)) + 1;
        buffer->cursor_col = amount > buffer->cursor_col ? 0 : buffer->cursor_col - amount;
    } else {
        size_t amount = (size_t)col_change;
        buffer->cursor_col = amount > length - (buffer->cursor_col > length ? length : buffer->cursor_col)
            ? length : buffer->cursor_col + amount;
    }
    if (buffer->cursor_col > length) buffer->cursor_col = length;
}
