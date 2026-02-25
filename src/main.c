#include "text_buffer.h"
#include "file_io.h"
#include "render.h"
#include "input.h"

int main() {
    TextBuffer *buffer = init_text_buffer();
    load_file(buffer, "example.c");

    enable_raw_mode();

    while (1) {
        render_text_buffer(buffer);
        process_keypress(buffer);
    }

    disable_raw_mode();
    free_text_buffer(buffer);
    return 0;
}