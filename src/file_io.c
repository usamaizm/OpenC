#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_io.h"
#include "../include/text_buffer.h"

#define MAX_LINE_LENGTH 256

// Load a file into the text buffer
int load_file(TextBuffer *buffer, const char *filename) {
    if (!buffer || !filename) return -1;

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
            if(len > 1 && line[len - 2] == '\r') line[len - 2] = '\0'; // handle \r\n
        }
        append_line(buffer, line);
    }

    fclose(file);
    return 0;
}

// Save the text buffer to a file
int save_file(TextBuffer *buffer, const char *filename) {
    if (!buffer || !filename) return -1;

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return -1;
    }

    for (int i = 0; i < buffer->num_lines; i++) {
        fprintf(file, "%s\n", buffer->lines[i]);
    }

    fclose(file);
    return 0;
}