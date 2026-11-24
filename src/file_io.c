#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "file_io.h"

static int replace_first_line(TextBuffer *buffer, const char *line) {
    size_t length = strlen(line);
    char *copy = malloc(length + 1);
    if (!copy) return -1;
    memcpy(copy, line, length + 1);
    free(buffer->lines[0]);
    buffer->lines[0] = copy;
    return 0;
}

int load_file(TextBuffer *buffer, const char *filename) {
    if (!buffer || !filename) return -1;
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    if (clear_text_buffer(buffer) != 0) {
        fclose(file);
        return -1;
    }

    char *line = NULL;
    size_t length = 0, capacity = 0;
    int ch, status = 0;
    int first_line = 1;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (length > 0 && line[length - 1] == '\r') --length;
            if (capacity <= length) {
                char *grown = realloc(line, length + 1);
                if (!grown) { status = -1; break; }
                line = grown;
                capacity = length + 1;
            }
            line[length] = '\0';
            if (first_line) {
                status = replace_first_line(buffer, line);
                first_line = 0;
            } else {
                status = append_line(buffer, line);
            }
            if (status != 0) break;
            length = 0;
        } else {
            if (length + 1 >= capacity) {
                size_t next = capacity ? capacity * 2 : 128;
                if (next <= capacity) { status = -1; break; }
                char *grown = realloc(line, next);
                if (!grown) { status = -1; break; }
                line = grown;
                capacity = next;
            }
            line[length++] = (char)ch;
        }
    }

    if (status == 0 && ferror(file)) status = -1;
    if (status == 0) {
        if (length > 0 && line[length - 1] == '\r') --length;
        if (capacity <= length) {
            char *grown = realloc(line, length + 1);
            if (!grown) status = -1;
            else { line = grown; capacity = length + 1; }
        }
        if (status == 0) {
            line[length] = '\0';
            if (first_line) status = replace_first_line(buffer, line);
            else status = append_line(buffer, line);
        }
    }

    free(line);
    if (fclose(file) != 0) status = -1;
    return status;
}

int save_file(TextBuffer *buffer, const char *filename) {
    if (!buffer || !filename) return -1;

    size_t filename_length = strlen(filename);
    if (filename_length > SIZE_MAX - sizeof(".tmp.XXXXXX")) return -1;

    struct stat existing;
    int has_existing = 0;
    mode_t existing_mode = 0;
    if (lstat(filename, &existing) == 0) {
        if (!S_ISREG(existing.st_mode)) return -1;
        has_existing = 1;
        existing_mode = existing.st_mode & 0777;
    } else if (errno != ENOENT) {
        return -1;
    }

    char *temporary = malloc(filename_length + sizeof(".tmp.XXXXXX"));
    if (!temporary) return -1;
    (void)snprintf(temporary, filename_length + sizeof(".tmp.XXXXXX"),
                   "%s.tmp.XXXXXX", filename);

    int fd = mkstemp(temporary);
    if (fd < 0) {
        free(temporary);
        return -1;
    }
    if (has_existing && fchmod(fd, existing_mode) != 0) {
        (void)close(fd);
        (void)unlink(temporary);
        free(temporary);
        return -1;
    }

    FILE *file = fdopen(fd, "wb");
    if (!file) {
        (void)close(fd);
        (void)unlink(temporary);
        free(temporary);
        return -1;
    }

    int status = 0;
    for (size_t i = 0; i < buffer->line_count; ++i) {
        size_t length = strlen(buffer->lines[i]);
        if (fwrite(buffer->lines[i], 1, length, file) != length ||
            (i + 1 < buffer->line_count && fputc('\n', file) == EOF)) {
            status = -1;
            break;
        }
    }
    if (status == 0 && fflush(file) != 0) status = -1;
    if (status == 0 && fsync(fd) != 0) status = -1;
    if (fclose(file) != 0) status = -1;
    if (status == 0 && rename(temporary, filename) != 0) status = -1;
    if (status != 0) (void)unlink(temporary);
    free(temporary);
    return status;
}
