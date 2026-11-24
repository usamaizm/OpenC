# Building Open C

**Current status:** The repository contains an early terminal-editor prototype, not yet a complete cross-platform IDE.

## Source layout

- `src/main.c` — current application entry point
- `src/text_buffer.c` — text-buffer implementation (currently inconsistent with the public header/API)
- `src/file_io.c` — file loading and saving
- `src/input.c` — terminal input handling (POSIX-specific)
- `src/render.c` — ANSI terminal rendering
- `include/` — public headers

## Current build limitation

A reliable build command is not published yet. The current source files appear to have interface mismatches: for example, the text-buffer implementation defines a private structure and functions that do not match the names used by the entry point and other modules. The input layer uses POSIX `unistd.h` and `termios.h`, so it is not currently cross-platform.

Do not treat this document as a successful build recipe.

## Recommended next engineering steps

1. Establish one canonical `TextBuffer` structure in `include/text_buffer.h`.
2. Make all implementations include the canonical header rather than redefining structures.
3. Reconcile function names and signatures across headers, implementations, and `main.c`.
4. Define ownership, allocation-failure, and error-return conventions.
5. Separate terminal backend code from editor core so Windows and other platforms can add a backend.
6. Add a build system and unit tests only after the interfaces are consistent.

## Validation

A contribution is not build-verified until it compiles from a clean checkout with documented compiler versions and the automated tests pass.
