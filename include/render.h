#ifndef RENDER_H
#define RENDER_H

#include "text_buffer.h"

// Render the visible portion of the text buffer
void render_text_buffer(const TextBuffer *buffer);

// Clear the terminal screen
void clear_screen(void);

// Move the cursor to a specific row and column
void move_cursor(int row, int col);

#endif // RENDER_H