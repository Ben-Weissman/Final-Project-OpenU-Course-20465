# Two-Pass Assembler

## Overview
This project implements a classic two-pass assembler for a small, imaginary machine. The assembler is written in ANSI C (C90) and transforms assembly source files into machine code while supporting macros, labels, data definitions, and external/entry symbols. The build is split into a preprocessing phase, a first pass that parses and records symbols, and a second pass that resolves addresses and emits output files.

## Key features
- **Macro-aware preprocessing** – The preprocessor expands user-defined macros, validates their names against reserved keywords, and writes the expanded source to a temporary `.am` file before the main assembly passes begin.
- **Symbol and data management** – During the first pass the assembler builds code (`IC`) and data (`DC`) arrays, records labels, and handles directives such as `.data`, `.string`, `.entry`, and `.extern`, reporting any syntax or semantic errors it encounters.
- **Relocation and output generation** – The second pass patches unresolved label references, tracks external symbol usage, and emits the final `.ob` object file along with optional `.ent` and `.ext` companion files when entry or external declarations are present.
- **Instruction set support** – The assembler understands 16 operation codes (`mov`, `cmp`, `add`, …, `stop`) and standard directives for data and string storage, enabling it to translate a wide range of input programs.
- **Sample sources** – Example inputs under `io_files/` provide ready-made assembly programs you can assemble and modify while exploring the toolchain.

## Repository layout
- `src/` – Core implementation and build scripts, including the `assembler` entry point and the supporting modules for preprocessing, parsing, machine-code emission, and error handling.
- `headers/` – Shared declarations, constants, and data-structure definitions consumed across the assembler modules.
- `io_files/` – Sample assembly sources and their preprocessed counterparts for quick experimentation.

## Building
Prerequisites are a POSIX-like environment with `gcc` and `make` installed. Compile the assembler by running the provided Makefile in the `src/` directory:

```bash
make -C src
```

The build produces the `assembler` executable inside `src/`. The Makefile targets compile each translation unit with C90 flags and then link them into the final binary.

## Usage
1. Prepare one or more assembly source files with the `.as` extension. The command line accepts the path **without** the `.as` suffix.
2. Run the assembler, pointing to the source names relative to the repository root (you can pass multiple files in a single invocation):

   ```bash
   ./src/assembler io_files/test
   ```

   The program preprocesses each argument, expands macros, and then performs both assembly passes, reporting any errors it finds along the way.

3. On success, the assembler emits the following artifacts alongside the input file:
   - `name.am` – Macro-expanded source produced by the preprocessor.
   - `name.ob` – Object file listing the encoded instructions and data, prefixed by the final instruction and data counts.
   - `name.ent` – Generated when `.entry` directives are present, containing the resolved addresses of entry labels.
   - `name.ext` – Generated when `.extern` directives appear, listing each external symbol reference and its address.

If the assembler detects errors during preprocessing or the first pass, it reports them with line numbers and skips generating output for the affected file, continuing with any remaining inputs.

## Assembly language notes
- **Directives** – Use `.data` for integers, `.string` for character arrays, `.entry` to expose symbols to other modules, and `.extern` to mark imported symbols.
- **Operations** – Supported opcodes are `mov`, `cmp`, `add`, `sub`, `lea`, `clr`, `not`, `inc`, `dec`, `jmp`, `bne`, `red`, `prn`, `jsr`, `rts`, and `stop`. Operands can be immediate values, labels, or registers (direct and indirect).【
- **Addressing and relocation** – Code addresses start at 100. The second pass resolves label addresses, marks external references, and outputs machine words with relocation flags for each instruction and operand.
- **Line length and naming** – Source lines are limited to 80 characters and label names to 31 characters, mirroring the original project specification.【

## Example
The repository ships with a minimal sample program:

```asm
MAIN: add r3, LIST
LIST: .data 6           ,            -9         ,1
 .data -100,
```

Running `./src/assembler io_files/test` preprocesses the source, assembles it, and produces the corresponding `.am` and `.ob` files in `io_files/`. Use this sample as a starting point when experimenting with the assembler.

## Extending the assembler
The modular design separates preprocessing, parsing, machine-code generation, and file emission, making it straightforward to extend. For example, you can add new directives in `parser.c` and update the instruction enumeration in `headers/constants.h` to support additional opcodes or addressing modes.【F:headers/constants.h†L7-L39】【F:src/first_pass.c†L37-L132】【F:src/machine_code.c†L18-L122】

## License
This repository does not currently include an explicit license. If you plan to distribute modified versions, please add appropriate licensing information.
