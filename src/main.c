#include <stdio.h>
#include <errno.h>
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

    errno = 0;
    FILE *probe = fopen(filename, "rb");
    if (probe) {
        if (fclose(probe) != 0 || load_file(buffer, filename) != 0) {
            fprintf(stderr, "OpenC: failed to load %s\\n", filename);
            free_text_buffer(buffer);
            return 1;
        }
    } else if (errno != ENOENT) {
        fprintf(stderr, "OpenC: unable to open %s\\n", filename);
        free_text_buffer(buffer);
        return 1;
    }

    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        fprintf(stderr, "OpenC: interactive mode requires a terminal\n");
        free_text_buffer(buffer);
        return 2;
    }

    if (enable_raw_mode() != 0) {
        fputs("OpenC: unable to enable terminal raw mode\n", stderr);
        free_text_buffer(buffer);
        return 1;
    }
    int action = 0;
    while (action == 0) {
        render_text_buffer(buffer);
        action = process_keypress(buffer);
    }
    disable_raw_mode();

    int status = action == 3 ? 1 : 0;
    if (action == 3) fputs("OpenC: input or buffer operation failed\n", stderr);
    if (action == 2 && save_file(buffer, filename) != 0) {
        fprintf(stderr, "OpenC: failed to save %s\n", filename);
        status = 1;
    }
    free_text_buffer(buffer);
    return status;
}
