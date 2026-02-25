// Implementation of text buffer functions needed for Milestone 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *buffer;
    size_t size;
    size_t capacity;
} TextBuffer;

TextBuffer *create_text_buffer(size_t initial_capacity) {
    TextBuffer *text_buffer = malloc(sizeof(TextBuffer));
    text_buffer->buffer = malloc(initial_capacity);
    text_buffer->size = 0;
    text_buffer->capacity = initial_capacity;
    return text_buffer;
}

void free_text_buffer(TextBuffer *text_buffer) {
    free(text_buffer->buffer);
    free(text_buffer);
}

void append_to_buffer(TextBuffer *text_buffer, const char *text) {
    size_t text_length = strlen(text);
    if (text_buffer->size + text_length + 1 > text_buffer->capacity) {
        text_buffer->capacity *= 2;
        text_buffer->buffer = realloc(text_buffer->buffer, text_buffer->capacity);
    }
    strcpy(text_buffer->buffer + text_buffer->size, text);
    text_buffer->size += text_length;
}

const char* get_buffer_content(TextBuffer *text_buffer) {
    return text_buffer->buffer;
}