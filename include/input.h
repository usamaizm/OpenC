#ifndef INPUT_H
#define INPUT_H

#include "text_buffer.h"

// Initialize raw mode for terminal input
void enable_raw_mode(void);

// Restore normal terminal input mode
void disable_raw_mode(void);

// Process a single keypress and update the text buffer
void process_keypress(TextBuffer *buffer);

#endif // INPUT_H