CC ?= cc
CFLAGS ?= -D_POSIX_C_SOURCE=200809L -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O2
CPPFLAGS ?= -Iinclude
TARGET := openc
SOURCES := src/main.c src/text_buffer.c src/file_io.c src/render.c src/input.c

.PHONY: all test clean
all: $(TARGET)

$(TARGET): $(SOURCES) include/text_buffer.h include/file_io.h include/render.h include/input.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@


test:
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_text_buffer.c src/text_buffer.c -o test_text_buffer
	./test_text_buffer
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_file_io.c src/text_buffer.c src/file_io.c -o test_file_io
	./test_file_io
	rm -f test_text_buffer test_file_io

clean:
	rm -f $(TARGET) $(TARGET).exe test_text_buffer test_file_io
