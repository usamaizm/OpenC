# Building OpenC

**Current status:** The repository builds a small POSIX terminal-editor prototype. It is not yet a complete cross-platform IDE.

## Prerequisites

- A C11-compatible compiler.
- GNU Make or a compatible Make implementation.
- A POSIX-like terminal environment for interactive use.

## Build and test

From the repository root:

```sh
make
make test
./openc path/to/file.c
make clean
```

The CI workflow builds the editor and runs the text-buffer and file-I/O tests on Ubuntu. See [the passing CI run](https://github.com/usamaizm/OpenC/actions/runs/45387578692).

## Source layout

- src/main.c — application entry point and lifecycle.
- src/text_buffer.c — canonical line-based text buffer.
- src/file_io.c — file loading and saving.
- src/input.c — POSIX terminal input.
- src/render.c — ANSI terminal rendering.
- include/ — public interfaces.
- tests/ — text-buffer and file-I/O tests.

## Current controls

- Arrow keys — move the cursor.
- Enter — split the current line.
- Backspace — delete before the cursor or join with the previous line.
- Ctrl-S — save the current file.
- Ctrl-Q — quit.

## Known limitations

- Terminal input currently depends on POSIX termios and unistd.
- Rendering is ANSI terminal output; no GUI backend exists.
- No syntax highlighting, compiler integration, project browser, debugger, or plugin host is implemented.
- There is no unsaved-change confirmation or save-as dialog.
- Terminal resize, scrolling, Unicode grapheme editing, and comprehensive terminal error recovery need additional work.
- File-I/O tests cover long lines, empty files, and newline round-tripping; broader filesystem failure tests remain needed.

## Validation rule

A contribution is build-verified only when the clean-checkout CI build and tests pass. A passing editor-core build does not establish cross-platform IDE support.
