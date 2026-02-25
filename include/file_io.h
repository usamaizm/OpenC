#ifndef FILE_IO_H
#define FILE_IO_H

#include "text_buffer.h"

int load_file(TextBuffer *buffer, const char *filename);
int save_file(TextBuffer *buffer, const char *filename);

#endif // FILE_IO_H