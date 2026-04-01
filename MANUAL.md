<div align="center">

# 📖 User Manual

## std-c99-proj-skill — Pi Coding Agent Skill

**Professional pure ANSI C99 development framework**

v2.0.0

---

</div>

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. Prerequisites](#2-prerequisites)
- [3. Installation](#3-installation)
- [4. Your First Project](#4-your-first-project)
- [5. Build Targets](#5-build-targets)
- [6. Actions Reference](#6-actions-reference)
  - [6.1 init — Scaffold a Project](#61-init--scaffold-a-project)
  - [6.2 build — Compile](#62-build--compile)
  - [6.3 test — Tests + Valgrind](#63-test--tests--valgrind)
  - [6.4 static-analysis — clang-tidy](#64-static-analysis--clang-tidy)
  - [6.5 docs — Doxygen](#65-docs--doxygen)
- [7. Project Structure](#7-project-structure)
- [8. Memory Arena Guide](#8-memory-arena-guide)
  - [8.1 Basics](#81-basics)
  - [8.2 API Reference](#82-api-reference)
  - [8.3 Patterns and Idioms](#83-patterns-and-idioms)
  - [8.4 Alignment](#84-alignment)
  - [8.5 Safety Guarantees](#85-safety-guarantees)
- [9. Framework Rules](#9-framework-rules)
- [10. Working with Pi](#10-working-with-pi)
  - [10.1 Natural Language Workflow](#101-natural-language-workflow)
  - [10.2 Direct Skill Invocation](#102-direct-skill-invocation)
  - [10.3 Multi-Target Workflow](#103-multi-target-workflow)
- [11. Intermediate Topics](#11-intermediate-topics)
  - [11.1 Adding Source Files](#111-adding-source-files)
  - [11.2 Adding Libraries](#112-adding-libraries)
  - [11.3 Adding More Tests](#113-adding-more-tests)
  - [11.4 Release Builds](#114-release-builds)
- [12. Advanced Topics](#12-advanced-topics)
  - [12.1 Custom Targets](#121-custom-targets)
  - [12.2 Multiple Arenas](#122-multiple-arenas)
  - [12.3 Arena-Based Data Structures](#123-arena-based-data-structures)
  - [12.4 CI/CD Integration](#124-cicd-integration)
- [13. Troubleshooting](#13-troubleshooting)
- [14. Uninstall](#14-uninstall)

---

## 1. Introduction

`std-c99-proj-skill` is a skill for the [Pi coding agent](https://github.com/badlogic/pi-mono) that provides a complete, opinionated framework for pure ANSI C99 development.

**What it gives you:**

- A project scaffold with memory arena allocator, strict compiler flags, and tests
- Containerized builds via Podman — your code compiles inside Linux containers, never on your host
- Mandatory Valgrind memory analysis on every test run
- Static analysis with clang-tidy
- Doxygen documentation generation
- Support for 7 build targets across RHEL, Debian, and Ubuntu

**Who is this for:**

- Developers writing portable, embedded-friendly, or systems-level C code
- Teams that need reproducible builds across multiple Linux distributions
- Anyone who wants zero-leak, zero-warning C code from day one

---

## 2. Prerequisites

Before installing, make sure you have:

| Tool | Purpose | Check |
|------|---------|-------|
| [Pi](https://github.com/badlogic/pi-mono) | AI coding agent | `pi --version` |
| [Podman](https://podman.io/) | Container runtime | `podman --version` |
| [Git](https://git-scm.com/) | Version control | `git --version` |

**Podman** runs in rootless mode — no root access needed. If you have Docker but not Podman, you can alias it:

```bash
alias podman=docker
```

> **Note:** The first build for each target downloads a container image (200–400 MB). Subsequent builds use the cached image and are near-instant.

---

## 3. Installation

### Install the skill

```bash
pi install git:github.com/ibitato/std-c99-proj-skill
```

### Verify

```bash
pi list
```

You should see:

```
User packages:
  git:github.com/ibitato/std-c99-proj-skill
    /home/<user>/.pi/agent/git/github.com/ibitato/std-c99-proj-skill
```

### Pin to a specific version

```bash
pi install git:github.com/ibitato/std-c99-proj-skill@v2.0.0
```

### Update

```bash
pi update
```

---

## 4. Your First Project

### Step 1 — Create a directory and start Pi

```bash
mkdir my-c99-project
cd my-c99-project
pi
```

### Step 2 — Initialize the project

Type in Pi:

```
/skill:std-c99-proj
Initialize a new C99 project
```

Pi will run `init_project.sh`, creating the full project scaffold with source files, tests, Containerfiles, and a git repository.

### Step 3 — Build

```
Build for RHEL 9
```

Pi will run `build.sh rhel9 Debug`, compiling your code inside a Rocky Linux 9 container.

### Step 4 — Test

```
Run tests for RHEL 9
```

Pi will run `test.sh rhel9`, executing all tests under Valgrind. You should see:

```
27/27 tests passed
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

**Congratulations** — you have a working, Valgrind-clean C99 project.

---

## 5. Build Targets

All builds run inside Podman containers. You choose the target; the skill handles the rest.

| Target | Distribution | Base Image | GCC Version |
|--------|-------------|------------|-------------|
| `rhel8` | RHEL 8 / Rocky 8 / Alma 8 | `rockylinux:8` | 8.x |
| `rhel9` | RHEL 9 / Rocky 9 / Alma 9 | `rockylinux:9` | 11.x |
| `rhel10` | RHEL 10 / Rocky 10 / Alma 10 | `quay.io/rockylinux/rockylinux:10` | 14.x |
| `debian11` | Debian 11 Bullseye | `debian:bullseye` | 10.x |
| `debian12` | Debian 12 Bookworm | `debian:bookworm` | 12.x |
| `ubuntu2204` | Ubuntu 22.04 LTS Jammy | `ubuntu:22.04` | 11.x |
| `ubuntu2404` | Ubuntu 24.04 LTS Noble | `ubuntu:24.04` | 13.x |

Each container includes: `gcc`, `clang`, `clang-tidy`, `cmake`, `make`, `git`, `valgrind`, `doxygen`.

---

## 6. Actions Reference

### 6.1 init — Scaffold a Project

Creates the full project structure in the current directory.

**In Pi:**
```
Initialize a new C99 project
```

**Direct:**
```bash
bash <SKILL_DIR>/scripts/init_project.sh
```

**What it creates:**

```
my-project/
├── CMakeLists.txt          # Build system (C99 strict)
├── Doxyfile                # Doxygen configuration
├── .gitignore
├── containers/
│   ├── Containerfile.rhel
│   └── Containerfile.debian
├── src/
│   ├── main.c              # Entry point
│   ├── mem_arena.c         # Arena allocator
│   └── utils.c             # Utilities
├── include/
│   ├── mem_arena.h         # Arena API
│   └── utils.h             # Utility headers
└── tests/
    └── test_arena.c        # 27 assertions
```

**Safety:** Refuses to run if `src/`, `include/`, `tests/`, or `CMakeLists.txt` already exist.

---

### 6.2 build — Compile

Compiles the project inside a container for the given target.

**In Pi:**
```
Build for debian12
Build for rhel9 in Release mode
```

**Direct:**
```bash
bash <SKILL_DIR>/scripts/build.sh <target> [Debug|Release]
```

**Examples:**
```bash
bash <SKILL_DIR>/scripts/build.sh rhel9 Debug      # Debug build
bash <SKILL_DIR>/scripts/build.sh ubuntu2404 Release # Optimized build
```

**Compiler flags (always applied):**
```
-std=c99 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wshadow -Wfloat-conversion
```

Any warning is a build error. No exceptions.

---

### 6.3 test — Tests + Valgrind

Builds with tests enabled, runs `ctest`, then runs every test binary under Valgrind.

**In Pi:**
```
Run tests for rhel9
```

**Direct:**
```bash
bash <SKILL_DIR>/scripts/test.sh <target>
```

**Valgrind flags:**
```
--leak-check=full --errors-for-leak-kinds=all --error-exitcode=1
```

Any leak or memory error is a **hard failure** — the script exits with a non-zero code.

---

### 6.4 static-analysis — clang-tidy

Runs clang-tidy on all `.c` and `.h` files inside the target container.

**In Pi:**
```
Run static analysis for debian12
```

**Direct:**
```bash
bash <SKILL_DIR>/scripts/static_analysis.sh <target>
```

The script generates `compile_commands.json` first (via CMake), then runs clang-tidy with `--warnings-as-errors="*"`.

---

### 6.5 docs — Doxygen

Generates HTML documentation from Doxygen comments in your code.

**In Pi:**
```
Generate documentation for rhel9
```

**Direct:**
```bash
bash <SKILL_DIR>/scripts/docs.sh <target>
```

Output goes to `docs/html/index.html`.

---

## 7. Project Structure

```
my-project/
├── CMakeLists.txt              # Build configuration
│   ├── C99 strict flags        #   -std=c99 -pedantic -Werror ...
│   ├── Main executable         #   from src/*.c
│   └── Tests (opt-in)          #   BUILD_TESTS=ON
├── Doxyfile                    # Documentation config
├── .gitignore
├── containers/
│   ├── Containerfile.rhel      #   ARG BASE_IMAGE (Rocky 8/9/10)
│   └── Containerfile.debian    #   ARG BASE_IMAGE (Debian/Ubuntu)
├── src/                        # Application source
│   ├── main.c                  #   Entry point
│   ├── mem_arena.c             #   Arena implementation
│   └── utils.c                 #   Utilities
├── include/                    # Public headers
│   ├── mem_arena.h             #   Arena API
│   └── utils.h                 #   Utility API
├── tests/                      # Test source
│   └── test_arena.c            #   Arena tests (27 assertions)
├── build/                      # Build output (gitignored)
└── docs/                       # Generated docs (gitignored)
```

**Adding files:** Put `.c` files in `src/`, headers in `include/`, tests in `tests/`. CMake picks them up automatically via `file(GLOB ...)`.

---

## 8. Memory Arena Guide

### 8.1 Basics

A memory arena is a simple allocator: one big `malloc` up front, then fast bump-pointer allocations. When you're done, one `free` releases everything.

```
Traditional:                    Arena:
malloc(A)                       init(arena, big_size)
malloc(B)                       alloc(arena, A)
malloc(C)                       alloc(arena, B)
...                             alloc(arena, C)
free(C)  ← easy to forget      ...
free(B)  ← easy to leak        free(arena)  ← one call, zero leaks
free(A)
```

**Why arenas:**
- Zero leaks by design — one `free` releases everything
- No fragmentation — allocations are contiguous
- Fast — each allocation is a pointer bump (O(1))
- Simple — no need to track individual allocations

### 8.2 API Reference

```c
#include "mem_arena.h"

MemArena arena;
arena.memory = NULL;                              /* required before first init */

int   mem_arena_init (MemArena *arena, size_t size);  /* 0 = ok, -1 = fail  */
void *mem_arena_alloc(MemArena *arena, size_t size);  /* NULL = fail        */
void  mem_arena_reset(MemArena *arena);               /* reuse without free */
void  mem_arena_free (MemArena *arena);               /* release everything */
```

| Function | What it does | Failure |
|----------|-------------|---------|
| `init` | Allocates backing buffer | Returns -1 (NULL arena, size 0, malloc fail, double init) |
| `alloc` | Bump-allocates `size` bytes (aligned) | Returns NULL (full, NULL arena, size 0, overflow) |
| `reset` | Sets `used = 0`, keeps buffer | — |
| `free` | Frees buffer, zeros struct | Idempotent (safe to call twice) |

### 8.3 Patterns and Idioms

**Basic usage:**

```c
MemArena arena;
arena.memory = NULL;
mem_arena_init(&arena, 1024 * 1024);  /* 1 MiB */

int *data = (int *)mem_arena_alloc(&arena, 100 * sizeof(int));
char *name = (char *)mem_arena_alloc(&arena, 256);

/* use data and name ... */

mem_arena_free(&arena);  /* one call frees everything */
```

**Reset and reuse (batch processing):**

```c
MemArena arena;
arena.memory = NULL;
mem_arena_init(&arena, 64 * 1024);

for (i = 0; i < num_batches; i++) {
    mem_arena_reset(&arena);           /* reuse buffer */
    buf = mem_arena_alloc(&arena, batch_size);
    process_batch(buf, batch_size);
}

mem_arena_free(&arena);
```

**Error handling:**

```c
MemArena arena;
arena.memory = NULL;
if (mem_arena_init(&arena, size) != 0) {
    fprintf(stderr, "Failed to initialize arena\n");
    return 1;
}

void *ptr = mem_arena_alloc(&arena, n);
if (ptr == NULL) {
    /* arena is full — free and bail, or reset and retry */
    mem_arena_free(&arena);
    return 1;
}
```

### 8.4 Alignment

All allocations are aligned to `MEM_ARENA_ALIGN` (8 bytes on most platforms). This means:

- Requesting 3 bytes consumes 8 bytes internally
- The returned pointer is always safe for `double`, `int`, `void *`, etc.
- No undefined behavior from misaligned access

```c
#define MEM_ARENA_ALIGN sizeof(union { double d; void *p; long long ll; })
```

### 8.5 Safety Guarantees

| Threat | Protection |
|--------|-----------|
| Misaligned access | All allocations aligned to `MEM_ARENA_ALIGN` |
| Integer overflow (`used + size`) | Overflow detected before any access |
| Double init (memory leak) | `init` rejects if `arena->memory != NULL` |
| Double free | `free` is idempotent — safe to call multiple times |
| Use after free | `alloc` returns NULL if `arena->memory == NULL` |
| Null arena pointer | All functions handle NULL gracefully |

---

## 9. Framework Rules

These rules are enforced as build errors — not warnings, not suggestions.

| # | Rule | Enforcement |
|---|------|-------------|
| 1 | **Pure C99** | `-std=c99 -pedantic -Werror` |
| 2 | **Zero warnings** | `-Wall -Wextra -Wconversion -Wshadow -Wfloat-conversion` |
| 3 | **No recursion** | Code review — use loops and explicit stacks |
| 4 | **Arena-only memory** | No direct `malloc`/`free` in application code |
| 5 | **Valgrind clean** | `--leak-check=full --error-exitcode=1` on every test |
| 6 | **Git required** | `init` creates the first commit |
| 7 | **Containerized** | All builds inside Podman — never on host |

**Why no recursion?** Predictable stack usage. No stack overflow risk. Every algorithm can be rewritten iteratively with an explicit stack.

**Why arena-only?** Zero leaks by design. The arena implementation itself uses `malloc`/`free` internally — application code never touches them.

---

## 10. Working with Pi

### 10.1 Natural Language Workflow

The skill is designed for natural language interaction. Pi reads the skill description and knows when to activate it.

```
> Create a new C99 project with memory arena
> Build it for Ubuntu 24.04
> Run the tests
> Now build for RHEL 9 in Release mode
> Run static analysis
> Generate the documentation
```

Pi handles the script invocations, error interpretation, and follow-up suggestions.

### 10.2 Direct Skill Invocation

Force-load the skill with the slash command:

```
/skill:std-c99-proj
```

Then give instructions:

```
/skill:std-c99-proj Initialize a project and test it on debian12
```

### 10.3 Multi-Target Workflow

```
> Build for rhel8, rhel9, and rhel10 sequentially
> Run tests on all three
```

Pi will execute each target one at a time, stopping on the first failure.

---

## 11. Intermediate Topics

### 11.1 Adding Source Files

1. Put `.c` files in `src/`
2. Put `.h` files in `include/`
3. Build — CMake picks them up automatically via `file(GLOB ...)`

```
> I added src/parser.c and include/parser.h. Build for rhel9.
```

**Convention:** Every `.h` file needs a header guard:

```c
#ifndef PARSER_H
#define PARSER_H
/* ... */
#endif /* PARSER_H */
```

### 11.2 Adding Libraries

Edit `CMakeLists.txt` to link external libraries:

```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE m)  # libm
```

For libraries that need `pkg-config`, add to the Containerfiles:

```dockerfile
RUN apt-get install -y libfoo-dev   # Debian/Ubuntu
RUN dnf install -y foo-devel        # RHEL
```

### 11.3 Adding More Tests

Create a new file in `tests/`, e.g., `tests/test_parser.c`:

```c
#include <stdio.h>
#include "parser.h"

int main(void) {
    /* your tests here */
    return 0;  /* 0 = pass, non-zero = fail */
}
```

CMake automatically discovers it. Each `tests/test_*.c` becomes a separate test binary and ctest entry.

### 11.4 Release Builds

```
> Build for rhel9 in Release mode
```

Release adds `-O2 -DNDEBUG` (CMake default). Debug adds `-g` with no optimization.

---

## 12. Advanced Topics

### 12.1 Custom Targets

To add a target (e.g., Fedora 41):

1. Determine which Containerfile family to use (RHEL → `Containerfile.rhel`, Debian → `Containerfile.debian`)
2. Add a case to the `build.sh` (and `test.sh`, `static_analysis.sh`, `docs.sh`):

```bash
fedora41) CFILE="containers/Containerfile.rhel"; BUILD_ARG="BASE_IMAGE=fedora:41";;
```

3. Test: `bash scripts/build.sh fedora41 Debug`

### 12.2 Multiple Arenas

Use separate arenas for different lifetimes:

```c
MemArena permanent;    /* lives for the entire program */
MemArena scratch;      /* reset every frame/iteration */

permanent.memory = NULL;
scratch.memory = NULL;
mem_arena_init(&permanent, 4 * 1024 * 1024);  /* 4 MiB */
mem_arena_init(&scratch, 256 * 1024);          /* 256 KiB */

Config *cfg = (Config *)mem_arena_alloc(&permanent, sizeof(Config));

for (i = 0; i < num_frames; i++) {
    mem_arena_reset(&scratch);
    TempData *tmp = (TempData *)mem_arena_alloc(&scratch, sizeof(TempData));
    process(cfg, tmp);
}

mem_arena_free(&scratch);
mem_arena_free(&permanent);
```

### 12.3 Arena-Based Data Structures

**Dynamic array (no recursion):**

```c
typedef struct {
    int   *data;
    size_t count;
    size_t capacity;
} IntArray;

IntArray arr;
arr.capacity = 1024;
arr.count = 0;
arr.data = (int *)mem_arena_alloc(&arena, arr.capacity * sizeof(int));

/* push */
if (arr.count < arr.capacity) {
    arr.data[arr.count++] = value;
}
```

**Stack-based tree traversal (no recursion):**

```c
typedef struct { TreeNode *node; int depth; } StackEntry;

StackEntry *stack = (StackEntry *)mem_arena_alloc(&arena, max_depth * sizeof(StackEntry));
size_t top = 0;

stack[top].node = root;
stack[top].depth = 0;
top++;

while (top > 0) {
    top--;
    TreeNode *n = stack[top].node;
    /* process n */
    if (n->right != NULL) { stack[top].node = n->right; stack[top].depth++; top++; }
    if (n->left  != NULL) { stack[top].node = n->left;  stack[top].depth++; top++; }
}
```

### 12.4 CI/CD Integration

The scripts work in any CI environment with Podman:

```yaml
# GitHub Actions example
jobs:
  build:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        target: [rhel8, rhel9, rhel10, debian12, ubuntu2404]
    steps:
      - uses: actions/checkout@v4
      - name: Install Podman
        run: sudo apt-get install -y podman
      - name: Build
        run: bash scripts/build.sh ${{ matrix.target }} Release
      - name: Test + Valgrind
        run: bash scripts/test.sh ${{ matrix.target }}
      - name: Static Analysis
        run: bash scripts/static_analysis.sh ${{ matrix.target }}
```

> **Note:** In CI, the scripts reference `containers/Containerfile.*` relative to the project root. Make sure the working directory is the project root.

---

## 13. Troubleshooting

### "Containerfile not found"

You're running a script from the wrong directory. Scripts expect to be run from the project root (where `containers/` lives).

```bash
cd my-project
bash <SKILL_DIR>/scripts/build.sh rhel9
```

### "Error: unknown target"

Check spelling. Valid targets: `rhel8`, `rhel9`, `rhel10`, `debian11`, `debian12`, `ubuntu2204`, `ubuntu2404`.

### Build fails with `-Werror`

A warning is being treated as an error. Common causes:

| Warning | Cause | Fix |
|---------|-------|-----|
| `-Wconversion` | Implicit narrowing (e.g., `int` to `char`) | Add explicit cast |
| `-Wshadow` | Variable shadows outer scope | Rename the variable |
| `-Wunused-function` | Static function not called | Remove it or use `(void)` |
| `-Wfloat-conversion` | Float to int without cast | Add explicit cast |

### Valgrind reports leaks

```
definitely lost: X bytes in Y blocks
```

You allocated memory outside the arena (direct `malloc`) or forgot `mem_arena_free`. Check that:

1. All dynamic memory goes through the arena
2. `mem_arena_free` is called before `return`
3. Error paths also call `mem_arena_free`

### "arena: already initialized"

You called `mem_arena_init` twice without `mem_arena_free` in between. Either free first or use `mem_arena_reset` to reuse.

### Podman permission errors

If you see `EPERM` or capability warnings, ensure Podman runs rootless:

```bash
podman info | grep rootless
```

The capability warnings (`can't raise ambient capability...`) are harmless in rootless mode.

### First build is slow

The first build for each target downloads the base image and installs packages (200–400 MB). Subsequent builds use the cached image. To pre-cache all targets:

```bash
for t in rhel8 rhel9 rhel10 debian11 debian12 ubuntu2204 ubuntu2404; do
    bash <SKILL_DIR>/scripts/build.sh $t Debug
done
```

---

## 14. Uninstall

### Remove the skill

```bash
pi remove git:github.com/ibitato/std-c99-proj-skill
```

### Clean cached container images

```bash
podman rmi $(podman images --filter "reference=std-c99-proj:*" -q) 2>/dev/null
```

Projects created with the skill are standalone — they keep working without the skill installed. The Containerfiles and scripts are copied into each project.

---

<div align="center">

**[GitHub](https://github.com/ibitato/std-c99-proj-skill)** · **[Agent Skills Standard](https://agentskills.io)** · **[Pi Coding Agent](https://github.com/badlogic/pi-mono)**

MIT License — © 2026 ibitato

</div>
