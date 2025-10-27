# Two-Pass Assembler

A compact ANSI C90 implementation of a macro-aware, two-pass assembler for a small teaching architecture. The tool expands user macros, records symbols during the first pass, resolves addresses in the second pass, and finally emits object, entry, and extern listings.

Note: After completing the course, I revisited this project using AI-assisted code review and debugging tools to refactor, document, and enhance overall code quality and maintainability.

## Build
- Prerequisites: `gcc`, `make`, and a POSIX-like shell.
- Build the executable with strict warnings enabled:
  ```bash
  make -C src CFLAGS="-std=c90 -ansi -Wall -Wextra -pedantic"
  ```
  The provided `src/Makefile` already compiles every translation unit with `-std=c90 -ansi -Wall -pedantic`; the extra `-Wextra` flag above is recommended for development builds.
- The resulting binary is written to `src/assembler`. Use `make -C src clean` to remove object files when needed.

## Usage
```bash
./src/assembler <path/to/source_without_extension> [...]
```
- Pass one or more file names **without** the `.as` suffix. Each source is preprocessed into `<name>.am`, assembled, and produces the following on success:
  - `<name>.ob` – Encoded instructions and data prefixed by the final instruction/data counters.
  - `<name>.ent` – Written when `.entry` directives exist, listing resolved entry labels.
  - `<name>.ext` – Written when `.extern` directives are referenced, capturing their usage addresses.
- Errors detected during preprocessing or the assembly passes are reported with line numbers and prevent output generation for that file, while the assembler continues with remaining inputs.

## Repository layout
- `src/` – All implementation files, including preprocessing, parsing, machine-code emission, and error handling modules.
- `headers/` – Shared declarations, constants, and data structure definitions.
- `io_files/` – Sample assembly inputs you can assemble immediately (`./src/assembler io_files/test`).

## Extending the assembler
The modular split between preprocessing, parsing, code generation, and file emission keeps feature work localised. New directives or opcodes typically involve updating the instruction tables in `headers/constants.h` and the relevant handlers in `src/parser.c`, `src/first_pass.c`, and `src/machine_code.c`.
