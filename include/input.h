#ifndef INPUT_H
#define INPUT_H

#include "text_buffer.h"

int enable_raw_mode(void);
void disable_raw_mode(void);
/* Returns 1 to quit, 2 to save, 3 on input/buffer failure, and 0 to continue. */
int process_keypress(TextBuffer *buffer);

#endif
