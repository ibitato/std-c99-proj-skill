# AGENTS.md — Pure ANSI C99 Project Rules

This project follows the std-c99-proj framework. All code contributions
(human or agent) must comply with these rules.

## Language

- Pure ANSI C99: `-std=c99 -pedantic`. No extensions, no C11+.
- All code must compile with zero warnings under:
  `-Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wshadow -Wfloat-conversion`

## Memory

- All dynamic memory through the arena API (`mem_arena_init/alloc/reset/free`).
- Never call `malloc`, `calloc`, `realloc`, or `free` in application code.
- Initialize `arena.memory = NULL` before first `mem_arena_init`.
- Always check return values: `init` returns -1 on failure, `alloc` returns NULL.

## Control Flow

- No recursion. Use iteration with explicit stacks when needed.
- No `goto`.

## Style

- Declare variables at the top of the block (C99 style).
- Cast `void *` returns from `mem_arena_alloc` explicitly.
- Use `(void)` on unused return values (e.g., `(void)printf(...)`).
- Header guards: `#ifndef FILENAME_H` / `#define FILENAME_H`.
- Document public functions with Doxygen comments (`\brief`, `\param`, `\return`).

## Files

- Source in `src/`, headers in `include/`, tests in `tests/`.
- Test files: `tests/test_*.c` — each with its own `main()`.

## Build & Test

- All builds run inside Podman containers. Never compile on host.
- Build: `bash <SKILL_DIR>/scripts/build.sh <target> [Debug|Release]`
- Test: `bash <SKILL_DIR>/scripts/test.sh <target>` — runs ctest + Valgrind.
- Every test must pass Valgrind with zero leaks and zero errors.
- Targets: `rhel8`, `rhel9`, `rhel10`, `debian11`, `debian12`, `ubuntu2204`, `ubuntu2404`.
