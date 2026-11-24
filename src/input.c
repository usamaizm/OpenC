#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "input.h"

static struct termios original_termios;
static int raw_mode_enabled = 0;

void disable_raw_mode(void) {
    if (raw_mode_enabled) {
        (void)tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
        raw_mode_enabled = 0;
    }
}

int enable_raw_mode(void) {
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) return -1;
    struct termios raw = original_termios;
    raw.c_lflag &= (tcflag_t)~(ECHO | ICANON);
    raw.c_iflag &= (tcflag_t)~(IXON | ICRNL);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) return -1;
    raw_mode_enabled = 1;
    if (atexit(disable_raw_mode) != 0) {
        disable_raw_mode();
        return -1;
    }
    return 0;
}

static int read_key(void) {
    unsigned char c;
    ssize_t count;
    do {
        count = read(STDIN_FILENO, &c, 1);
    } while (count == -1 && errno == EINTR);
    return count == 1 ? (int)c : -1;
}

/* Returns 1 to quit, 2 to save, and 0 to continue editing. */
int process_keypress(TextBuffer *buffer) {
    int c = read_key();
    if (c < 0) return 3;
    if (c == 17) return 1; /* Ctrl-Q */
    if (c == 19) return 2; /* Ctrl-S */

    switch (c) {
        case 127:
        case 8:
            if (delete_character(buffer) != 0) return 3;
            break;
        case '\r':
        case '\n':
            if (insert_newline(buffer) != 0) return 3;
            break;
        case 27: {
            int first = read_key();
            int second = read_key();
            if (first == '[') {
                switch (second) {
                    case 'A': move_cursor(buffer, -1, 0); break;
                    case 'B': move_cursor(buffer, 1, 0); break;
                    case 'C': move_cursor(buffer, 0, 1); break;
                    case 'D': move_cursor(buffer, 0, -1); break;
                }
            }
            break;
        }
        default:
            if (c >= 32 && c <= 126 && insert_character(buffer, (char)c) != 0) return 3;
            break;
    }
    return 0;
}
