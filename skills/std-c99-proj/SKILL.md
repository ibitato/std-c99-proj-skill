---
name: std-c99-proj
description: >
  Professional pure ANSI C99 project framework with memory arena allocation,
  containerized builds via Podman, mandatory Valgrind memory analysis, and
  clang-tidy static analysis. Enforces strict rules: no warnings (-Werror),
  no recursion (iteration only), no memory leaks (arena pattern), and git
  version control. Supports RHEL 8/9/10 and Debian 11/12 and Ubuntu 22.04/24.04
  build targets. Use when the user wants to create, build, test, or analyze
  a pure C99 project, or mentions memory arena, C99, Valgrind, or containerized
  C builds.
license: MIT
compatibility: Requires podman and git. Targets run inside containers.
---

# std-c99-proj — Pure ANSI C99 + Memory Arena Framework

Professional framework for pure ANSI C99 projects. All builds, tests, and
analysis run inside Podman containers for reproducibility.

## Philosophy

- **Pure C99**: `-std=c99 -pedantic` — no extensions, no C11+
- **Memory Arena**: single `malloc` per arena, bulk `free` — zero leaks by design
- **No recursion**: iteration only — predictable stack usage
- **Zero tolerance**: `-Werror -Wall -Wextra -Wconversion -Wshadow` — no warnings allowed
- **Valgrind mandatory**: every test run checks for leaks and memory errors
- **Git mandatory**: every project starts with version control
- **Containerized**: builds never touch the host — fully reproducible

## Available Actions

### 1. Initialize a project

Run the init script to scaffold a new C99 project in the current directory:

```bash
bash SKILL_DIR/scripts/init_project.sh
```

This creates: `src/`, `include/`, `tests/`, `containers/`, `CMakeLists.txt`,
`.gitignore`, template source files (memory arena, utils, main, tests), and
initializes a git repository with an initial commit.

**Do not run init in a directory that already has source files.**

### 2. Build

```bash
bash SKILL_DIR/scripts/build.sh <target> [Debug|Release]
```

Targets: `rhel8`, `rhel9`, `rhel10`, `debian11`, `debian12`, `ubuntu2204`, `ubuntu2404`

Default build type is `Debug`. The script builds a Podman image for the target
(if not cached) and compiles inside the container. The `build/` directory is
created in the project root.

### 3. Test with Valgrind

```bash
bash SKILL_DIR/scripts/test.sh <target>
```

Builds with `BUILD_TESTS=ON`, runs `ctest`, then runs every test binary under
Valgrind with `--leak-check=full --error-exitcode=1`. Any leak or memory error
is a hard failure.

### 4. Static analysis

```bash
bash SKILL_DIR/scripts/static_analysis.sh <target>
```

Runs `clang-tidy` inside the target container on all `.c` files in `src/` and
`include/`.

### 5. Generate documentation

```bash
bash SKILL_DIR/scripts/docs.sh <target>
```

Runs Doxygen inside the target container. Output goes to `docs/`.

## Target Catalog

| Target      | Family | Base Image        |
|-------------|--------|-------------------|
| `rhel8`     | RHEL   | `rockylinux:8`    |
| `rhel9`     | RHEL   | `rockylinux:9`    |
| `rhel10`    | RHEL   | `rockylinux:10`   |
| `debian11`  | Debian | `debian:bullseye` |
| `debian12`  | Debian | `debian:bookworm` |
| `ubuntu2204`| Debian | `ubuntu:22.04`    |
| `ubuntu2404`| Debian | `ubuntu:24.04`    |

See [references/TARGETS.md](references/TARGETS.md) for full details.

## Framework Rules

1. All code must compile with `-std=c99 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wshadow -Wfloat-conversion`
2. No recursion — use iteration (loops, explicit stacks)
3. Memory management via arena only — no direct `malloc`/`free` in application code
4. All tests must pass Valgrind with zero leaks and zero errors
5. Git repository required — init creates the first commit
6. All builds run inside Podman containers — never compile on host

See [references/RULES.md](references/RULES.md) for rationale and examples.

## Memory Arena API

```c
int   mem_arena_init(MemArena *arena, size_t size);  // returns 0 on success
void *mem_arena_alloc(MemArena *arena, size_t size);  // returns NULL on failure
void  mem_arena_reset(MemArena *arena);               // resets used to 0
void  mem_arena_free(MemArena *arena);                // frees backing memory
```

See [references/ARENA_API.md](references/ARENA_API.md) for full documentation.

## Project Layout After Init

```
my-project/
├── CMakeLists.txt
├── .gitignore
├── Doxyfile
├── containers/
│   ├── Containerfile.rhel
│   └── Containerfile.debian
├── src/
│   ├── main.c
│   ├── mem_arena.c
│   └── utils.c
├── include/
│   ├── mem_arena.h
│   └── utils.h
└── tests/
    └── test_arena.c
```

## SKILL_DIR

When referencing scripts and assets, `SKILL_DIR` is the directory containing
this `SKILL.md` file. The agent should resolve it before running commands.
