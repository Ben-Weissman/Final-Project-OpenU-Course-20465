# Code Review Summary

This review focuses on functional correctness, memory/resource management, API design, and presentation issues that would block using the assembler as portfolio-quality code.

## Critical correctness & safety
- `duplicate_string` allocates `sizeof(string)` bytes (i.e., pointer size) instead of `strlen(string) + 1`, so copying longer tokens corrupts memory when recording label references. 【F:src/utilities.c†L62-L65】
- `new_array` zeroes only `size` bytes (not `size * sizeof(short)`) and leaves `label_name_arr` uninitialised, so later reads in `get_IC_label` access garbage pointers. 【F:src/data_structures.c†L249-L292】
- `second_pass` iterates `i <= IC_end`, but `IC_end` already equals `get_index_special_array`, so the loop runs one past the last valid element and dereferences beyond the IC buffer. 【F:src/second_pass.c†L15-L35】
- File-name builders rely on `strcat` over freshly allocated buffers that never receive an initial terminator (e.g., `first_pass` and `pre_proccesor`), triggering undefined behaviour before the assembler even opens files. Initialise with `strcpy`/`snprintf` or `calloc`. 【F:src/first_pass.c†L24-L35】【F:src/preprocessor.c†L150-L167】【F:src/second_pass.c†L63-L113】
- `.data` parsing writes into a `short*` allocated with `sizeof(int)` bytes per element, so the allocator returns too little storage when `sizeof(int) > sizeof(short)`. 【F:src/parser.c†L206-L245】
- External appearance bookkeeping iterates `i <= MAX_APPEARANCES`, overrunning the last slot on every insert. 【F:src/data_structures.c†L378-L399】
- `get_instruction` defaults to `.entry` on any unknown token, masking typos instead of raising an error. 【F:src/parser.c†L114-L128】

## Resource management & robustness
- `new_label_table` returns uninitialised memory; `table->head` holds garbage until the first insertion, leading to random crashes. Ensure the struct is zeroed. 【F:src/data_structures.c†L118-L139】
- Most allocation helpers (`new_macro_list`, `new_array`, `new_ext_label_node`, etc.) skip NULL checks and leak partially constructed state on error paths (e.g., `create_new_macro` frees the struct but not `macro_name`). Add consistent failure handling and cleanup. 【F:src/data_structures.c†L71-L110】【F:src/data_structures.c†L249-L315】【F:src/data_structures.c†L348-L399】
- `is_label_name_exists` allocates a temporary copy on every lookup but never frees it when returning early, leaking on the hot path. 【F:src/data_structures.c†L213-L243】
- `end_cycle` only frees the container structs, leaking every dynamically allocated member array and string. Provide destructors for each structure instead of raw `free`. 【F:src/utilities.c†L125-L133】
- `open_file` returns NULL on failure, yet callers immediately use the pointer without checking, so missing input files produce segmentation faults instead of diagnostics. 【F:src/utilities.c†L74-L82】【F:src/first_pass.c†L24-L38】

## Logic & UX issues
- Command validation treats a missing second operand as “unused” rather than an error, so statements like `mov r1` slip through. Extend `is_cmd_args_valid` to reject `UNUSED_OPERARND` where the ISA requires operands. 【F:src/parser.c†L29-L101】
- Register encoding loops iterate one bit too many (`<= 6`) for target operands, setting a stray bit in the machine word. 【F:src/machine_code.c†L62-L111】
- Macro expansion buffers are reused with `memset(..., strlen(buf))`, which clears zero bytes the first time (because the string is empty) and leaves stale contents to be concatenated. Track buffer lengths explicitly or `memset` by capacity. 【F:src/preprocessor.c†L200-L238】
- Processing state such as `pre_proccesor_error_status` and allocated macro lists never reset after a failure, so the outer loop leaks and may double-process stale data. Consider RAII-style cleanup per iteration. 【F:src/assembler.c†L23-L36】

## Naming, typos, and documentation polish
- The public headers shipped in this repo contain numerous typos (`UNDERFINED_TYPE`, `pre_proccesor`, `Lenghts`, etc.) that weaken credibility; sweeping spelling fixes would help. 【F:headers/constants.h†L8-L100】【F:src/assembler.c†L23-L36】
- README and error messages also include spelling mistakes (“correspand”, “apppeances”)—clean copy signals professionalism. 【F:src/error_handling.c†L11-L33】【F:src/second_pass.c†L28-L35】
- Consider running the codebase through `clang-format`/`clang-tidy`; inconsistent spacing and missing `static` qualifiers for internal helpers make the build feel unfinished.

Addressing the above will significantly improve correctness, stability, and presentation for portfolio use.
