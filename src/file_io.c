#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"

int load_file(TextBuffer *buffer, const char *filename) {
    if (!buffer || !filename) return -1;
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;

    char *line = NULL;
    size_t length = 0, capacity = 0;
    int ch, status = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (length > 0 && line[length - 1] == '\r') --length;
            if (capacity <= length) {
                char *grown = realloc(line, length + 1);
                if (!grown) { status = -1; break; }
                line = grown; capacity = length + 1;
            }
            line[length] = '\0';
            if (append_line(buffer, line) != 0) { status = -1; break; }
            length = 0;
        } else {
            if (length + 1 >= capacity) {
                size_t next = capacity ? capacity * 2 : 128;
                if (next <= capacity) { status = -1; break; }
                char *grown = realloc(line, next);
                if (!grown) { status = -1; break; }
                line = grown; capacity = next;
            }
            line[length++] = (char)ch;
        }
    }
    if (status == 0 && ferror(file)) status = -1;
    if (status == 0 && length > 0) {
        if (length > 0 && line[length - 1] == '\r') --length;
        line[length] = '\0';
        if (append_line(buffer, line) != 0) status = -1;
    }
    free(line);
    if (fclose(file) != 0) status = -1;
    return status;
}

int save_file(TextBuffer *buffer, const char *filename) {
    if (!buffer || !filename) return -1;
    FILE *file = fopen(filename, "wb");
    if (!file) return -1;
    int status = 0;
    for (size_t i = 0; i < buffer->line_count; ++i) {
        size_t length = strlen(buffer->lines[i]);
        if (fwrite(buffer->lines[i], 1, length, file) != length ||
            (i + 1 < buffer->line_count && fputc('\n', file) == EOF)) {
            status = -1;
            break;
        }
    }
    if (fclose(file) != 0) status = -1;
    return status;
}
