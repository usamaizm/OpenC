#ifndef INPUT_H
#define INPUT_H

#include "text_buffer.h"

void enable_raw_mode(void);
void disable_raw_mode(void);
/* Returns 1 to quit, 2 to save, and 0 to continue editing. */
int process_keypress(TextBuffer *buffer);

#endif
