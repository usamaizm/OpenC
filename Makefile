CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O2
CPPFLAGS ?= -Iinclude
TARGET := openc
SOURCES := src/main.c src/text_buffer.c src/file_io.c src/render.c src/input.c

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(SOURCES) include/text_buffer.h include/file_io.h include/render.h include/input.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(TARGET) $(TARGET).exe
