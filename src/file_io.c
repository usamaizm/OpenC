#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"

// Load a file into the text buffer
int load_file(TextBuffer *buffer, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return -1;
    }

    char line[MAX_COLUMNS];
    while (fgets(line, MAX_COLUMNS, file)) {
        // Remove newline character
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        append_line(buffer, line);
    }

    fclose(file);
    return 0; // Success
}

// Save the text buffer to a file
int save_file(const TextBuffer *buffer, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return -1;
    }

    for (int i = 0; i < buffer->num_lines; i++) {
        fprintf(file, "%s\n", buffer->lines[i]);
    }

    fclose(file);
    return 0; // Success
}