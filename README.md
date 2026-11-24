# OpenC

**OpenC is a C-first terminal text-editor prototype and the foundation for a future cross-platform C IDE.**

The current implementation is a small POSIX terminal editor. It supports line-oriented text editing, cursor movement, file loading/saving, and a minimal keyboard workflow. A full IDE, syntax highlighting, compiler integration, GUI, and Windows terminal backend are future work—not current capabilities.

## Current status

- C11 source code.
- POSIX terminal input using termios and unistd.
- Dynamic line-based text buffer.
- File loading and saving, including long lines and final-newline preservation.
- Arrow-key cursor movement, Enter, Backspace, Ctrl-S save, Ctrl-Q quit.
- Automated tests for text-buffer operations and file I/O.

## Build and test

Requires a C11 compiler and Make on a POSIX-like environment.

```sh
make
make test
./openc path/to/file.c
make clean
```

The editor starts with an empty buffer if the requested file does not exist. Press Ctrl-S to save and Ctrl-Q to exit. The current editor does not yet provide a save confirmation dialog or unsaved-change prompt.

## Source layout

- include/text_buffer.h and src/text_buffer.c — line storage, editing, and cursor state.
- include/file_io.h and src/file_io.c — file loading and saving.
- include/input.h and src/input.c — POSIX raw-terminal input and key handling.
- include/render.h and src/render.c — ANSI terminal rendering.
- src/main.c — application lifecycle.
- tests/ — text-buffer and file-I/O tests.

## Architecture direction

The long-term goal is a C-first IDE for building cross-platform C applications. Keep the core editor independent from compiler integrations and platform-specific user interfaces. Introduce platform backends behind explicit interfaces rather than spreading POSIX assumptions through the editor core.

## Roadmap

1. Expand editing operations and test coverage.
2. Add robust terminal resizing, scrolling, and unsaved-change handling.
3. Add syntax-aware editing and project/file navigation.
4. Define compiler adapters for GCC and Clang.
5. Introduce a platform abstraction and additional input/rendering backends.
6. Add project templates, diagnostics, and debugging support.

## Engineering notes

- [BUILDING.md](BUILDING.md) contains current build notes.
- [ENGINEERING_AUDIT.md](ENGINEERING_AUDIT.md) records issues to revisit.
- [Ecosystem integration contract](https://github.com/usamaizm/AIDB/blob/main/docs/ECOSYSTEM_INTEGRATION.md) describes how OpenC fits into the wider project set.

## License

See [LICENSE](LICENSE).
