#define _POSIX_C_SOURCE 200809L
#include "file_io.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static void temporary_path(char *path) {
    int fd = mkstemp(path);
    assert(fd >= 0);
    assert(close(fd) == 0);
}

int main(void) {
    char input_path[] = "/tmp/openc-input-XXXXXX";
    char output_path[] = "/tmp/openc-output-XXXXXX";
    temporary_path(input_path);
    temporary_path(output_path);

    char long_line[601];
    memset(long_line, 'x', 600);
    long_line[600] = '\0';

    FILE *input = fopen(input_path, "wb");
    assert(input != NULL);
    assert(fwrite(long_line, 1, 600, input) == 600);
    assert(fwrite("\nlast", 1, 5, input) == 5);
    assert(fclose(input) == 0);

    TextBuffer *buffer = init_text_buffer();
    assert(buffer != NULL);
    assert(append_line(buffer, "old content") == 0);
    assert(load_file(buffer, input_path) == 0);
    assert(buffer->line_count == 2);
    assert(strcmp(buffer->lines[0], long_line) == 0);
    assert(strcmp(buffer->lines[1], "last") == 0);

    struct stat before_save;
    assert(stat(output_path, &before_save) == 0);
    assert(save_file(buffer, output_path) == 0);
    struct stat after_save;
    assert(stat(output_path, &after_save) == 0);
    assert((before_save.st_mode & 0777) == (after_save.st_mode & 0777));
    FILE *output = fopen(output_path, "rb");
    assert(output != NULL);
    char actual[700];
    size_t count = fread(actual, 1, sizeof(actual), output);
    assert(fclose(output) == 0);
    assert(count == 605);
    assert(memcmp(actual, long_line, 600) == 0);
    assert(memcmp(actual + 600, "\nlast", 5) == 0);

    FILE *trailing_newline = fopen(input_path, "wb");
    assert(trailing_newline != NULL);
    assert(fwrite("tail\n", 1, 5, trailing_newline) == 5);
    assert(fclose(trailing_newline) == 0);
    assert(load_file(buffer, input_path) == 0);
    assert(buffer->line_count == 2);
    assert(strcmp(buffer->lines[0], "tail") == 0);
    assert(strcmp(buffer->lines[1], "") == 0);
    assert(save_file(buffer, output_path) == 0);
    FILE *newline_output = fopen(output_path, "rb");
    assert(newline_output != NULL);
    char newline_bytes[6];
    assert(fread(newline_bytes, 1, sizeof(newline_bytes), newline_output) == 5);
    assert(memcmp(newline_bytes, "tail\n", 5) == 0);
    assert(fclose(newline_output) == 0);

    FILE *empty = fopen(input_path, "wb");
    assert(empty != NULL);
    assert(fclose(empty) == 0);
    assert(load_file(buffer, input_path) == 0);
    assert(buffer->line_count == 1);
    assert(strcmp(buffer->lines[0], "") == 0);

    char target_path[] = "/tmp/openc-target-XXXXXX";
    temporary_path(target_path);
    FILE *target = fopen(target_path, "wb");
    assert(target != NULL);
    assert(fwrite("keep-original", 1, 13, target) == 13);
    assert(fclose(target) == 0);

    char link_path[] = "/tmp/openc-link-XXXXXX";
    temporary_path(link_path);
    assert(unlink(link_path) == 0);
    assert(symlink(target_path, link_path) == 0);
    assert(append_line(buffer, "must not replace symlink target") == 0);
    assert(save_file(buffer, link_path) != 0);
    FILE *target_check = fopen(target_path, "rb");
    assert(target_check != NULL);
    char target_bytes[14] = {0};
    assert(fread(target_bytes, 1, 13, target_check) == 13);
    assert(fclose(target_check) == 0);
    assert(strcmp(target_bytes, "keep-original") == 0);

    free_text_buffer(buffer);
    assert(unlink(link_path) == 0);
    assert(unlink(target_path) == 0);
    assert(unlink(input_path) == 0);
    assert(unlink(output_path) == 0);
    return 0;
}
