# Framework Rules

## Mandatory Rules

Every project created with this skill must follow these rules. Violations are
build errors — not warnings.

### 1. Pure ANSI C99

All code compiles with:

```
-std=c99 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wshadow -Wfloat-conversion
```

No compiler extensions. No C11+ features. No GNU extensions.

**Implications:**
- Declare loop variables before the `for` statement
- Use `(void)` casts on unused return values (e.g., `(void)printf(...)`)
- Explicit casts for all implicit conversions that trigger `-Wconversion`

### 2. No Recursion

All algorithms must use iteration. No function may call itself directly or
indirectly.

**Rationale:** Predictable stack usage. No stack overflow risk. Easier to
reason about memory consumption.

**Alternative patterns:**
- Explicit stack (array + index) for tree/graph traversal
- Loop with state machine for recursive descent
- Iterative deepening for search algorithms

### 3. Memory Arena Only

Application code must not call `malloc`, `calloc`, `realloc`, or `free`
directly. All dynamic memory goes through the arena API.

**Rationale:** Zero leaks by design. Single allocation, single free. No
fragmentation. Deterministic memory usage.

**Exception:** The arena implementation itself uses `malloc`/`free` internally.

### 4. Valgrind Clean

Every test run must pass Valgrind with:

```
valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=1
```

Zero leaks. Zero errors. Any Valgrind finding is a hard failure.

### 5. Git Version Control

Every project starts with `git init` and an initial commit.

### 6. Containerized Builds

All compilation, testing, and analysis runs inside Podman containers. The host
system is never used for building.

## Compiler Flags

| | Debug | Release |
|---|---|---|
| Optimization | `-O0` | `-O2` |
| Debug info | `-g3` (full + macros) | — |
| Warnings | `-Wall -Wextra -Werror -Wconversion -Wshadow -Wfloat-conversion -pedantic` | same |
| Static analysis | `-fanalyzer` (GCC ≥ 10) | — |
| Stack protection | `-fstack-protector-strong` | `-fstack-protector-strong` |
| Buffer overflow | — | `-D_FORTIFY_SOURCE=2` |
| PIE | — | `-fPIE` |
| NDEBUG | — | `-DNDEBUG` |

`-fanalyzer` is automatically enabled on GCC ≥ 10 (all targets except rhel8).
It catches use-after-free, null dereference, double-free, and buffer overflows
at compile time.

## Coding Conventions

- All public functions and types documented with Doxygen comments
- Header guards use `#ifndef FILENAME_H` / `#define FILENAME_H` pattern
- NULL checks on all pointer parameters
- Error returns: `int` functions return 0 on success, -1 on failure;
  pointer functions return NULL on failure
- Initialize `arena.memory = NULL` before first `mem_arena_init`
