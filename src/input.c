#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "input.h"

static struct termios orig_termios;

// Restore terminal settings on exit
void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

// Enable raw mode (disable echo and canonical input)
void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);  // Restore on exit

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG); // Disable echo, canonical mode, signals
    raw.c_iflag &= ~(IXON | ICRNL);         // Disable Ctrl-S/Q, carriage return
    raw.c_cc[VMIN] = 1;  // Minimum number of bytes before read returns
    raw.c_cc[VTIME] = 0; // No timeout

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

// Helper function to read a single character
static char read_key() {
    char c;
    if (read(STDIN_FILENO, &c, 1) == -1) exit(1);
    return c;
}

// Process a single keypress
void process_keypress(TextBuffer *buffer) {
    char c = read_key();

    switch (c) {
        case 127: // Backspace
            if (buffer->cursor_col > 0 || buffer->cursor_row > 0) {
                delete_char(buffer);
            }
            break;

        case '\r': // Enter
        case '\n':
            insert_newline(buffer);
            break;

        case 27: { // Escape sequences (arrows)
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) break;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) break;

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': // Up
                        move_cursor(buffer, -1, 0);
                        break;
                    case 'B': // Down
                        move_cursor(buffer, 1, 0);
                        break;
                    case 'C': // Right
                        move_cursor(buffer, 0, 1);
                        break;
                    case 'D': // Left
                        move_cursor(buffer, 0, -1);
                        break;
                }
            }
            break;
        }

        default:
            // Printable characters
            if (c >= 32 && c <= 126) {
                insert_char(buffer, c);
            }
            break;
    }
}