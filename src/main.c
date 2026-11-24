#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "text_buffer.h"
#include "file_io.h"
#include "render.h"
#include "input.h"

int main(int argc, char **argv) {
    const char *filename = argc > 1 ? argv[1] : "untitled.c";
    TextBuffer *buffer = init_text_buffer();
    if (!buffer) {
        fputs("OpenC: unable to allocate text buffer\n", stderr);
        return 1;
    }

    FILE *probe = fopen(filename, "rb");
    if (probe) {
        fclose(probe);
        if (load_file(buffer, filename) != 0) {
            fprintf(stderr, "OpenC: failed to load %s\n", filename);
            free_text_buffer(buffer);
            return 1;
        }
    }

    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        fprintf(stderr, "OpenC: interactive mode requires a terminal\n");
        free_text_buffer(buffer);
        return 2;
    }

    enable_raw_mode();
    int action = 0;
    while (action == 0) {
        render_text_buffer(buffer);
        action = process_keypress(buffer);
    }
    disable_raw_mode();

    int status = 0;
    if (action == 2 && save_file(buffer, filename) != 0) {
        fprintf(stderr, "OpenC: failed to save %s\n", filename);
        status = 1;
    }
    free_text_buffer(buffer);
    return status;
}
