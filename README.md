Open C

A Pure-C, Cross-Platform IDE Built From Scratch

Open C is a C-first IDE designed to create cross-platform C applications. Every core component — text buffer, file management, syntax highlighting — is written entirely in pure C, with no external GUI frameworks or libraries. Compiler integration (GCC/Clang) is optional and modular, allowing the IDE to function fully even without any compiler installed.

This README serves as a guide for contributors and AI assistants to understand the architecture, purpose, and development plan of Open C.

⸻

Key Modules

Module	Purpose
text_buffer.c / text_buffer.h	Core text storage, cursor tracking, line management, and editing operations.
file_io.c / file_io.h	Loading and saving files into the text buffer.
render.c / render.h	OS-abstracted rendering of text to terminal or GUI framebuffer.
input.c / input.h	Low-level keyboard/mouse input handling for different platforms.
syntax.c / syntax.h	Tokenization and minimal syntax highlighting.
plugins.c / plugins.h	Optional compiler plugin system (GCC, Clang, etc.).
main.c	Entry point to initialize and run the IDE.


⸻

Development Philosophy
	1.	From Scratch – The IDE core is hand-coded in pure C; no external GUI libraries or frameworks are used.
	2.	Modular Design – Each component is independent and interacts through clear interfaces.
	3.	Compiler Plugins Optional – Open C works fully for editing and syntax highlighting even without a compiler.
	4.	Cross-Platform – Minimal OS-specific code, abstracted for Linux, Windows, and macOS.

⸻

Roadmap

Milestone 1: Core Text Buffer
	•	Dynamic text storage and line management.
	•	Cursor movement, insertion/deletion of characters and lines.
	•	Ready to connect to rendering and input modules.

Milestone 2: File I/O
	•	Load files into text buffer.
	•	Save buffer contents to disk.

Milestone 3: Rendering Engine
	•	Terminal-based first, optional GUI framebuffer later.
	•	Display visible lines and cursor position.

Milestone 4: Syntax Highlighting
	•	Basic C keyword highlighting using ANSI colors in the terminal.

Milestone 5: Compiler Plugins
	•	Optional GCC/Clang integration using modular plugin interface.

Milestone 6: Advanced Features
	•	Multi-file projects, project templates, and debugging support.

⸻

Example Usage (for Copilot or contributors)

#include "text_buffer.h"
#include "file_io.h"

int main() {
    TextBuffer *buffer = init_text_buffer();

    // Load a file into the text buffer
    load_file(buffer, "example.c");

    // Display the buffer contents
    print_text_buffer(buffer);

    // Save changes to disk
    save_file(buffer, "output.c");

    free_text_buffer(buffer);
    return 0;
}


⸻

Contribution Guidelines
	•	Follow the pure-C, from-scratch philosophy.
	•	Keep modules modular and clearly named.
	•	Document new features and functions in the README and header files.
	•	Open issues or submit pull requests for code improvements or bug fixes.

⸻

License

Open C is released under the MIT License, allowing open-source collaboration and commercial usage.