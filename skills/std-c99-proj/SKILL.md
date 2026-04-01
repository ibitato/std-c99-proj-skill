---
name: std-c99-proj
description: >
  Professional pure ANSI C99 project framework with hardened memory arena,
  containerized builds via Podman, mandatory Valgrind, -fanalyzer (GCC>=10),
  and clang-tidy. Enforces: -Werror, no recursion, no leaks, arena-only memory.
  Per-target output in build/<target>/. Generates AGENTS.md for auto-rules.
  Targets: RHEL 8/9/10, Debian 11/12, Ubuntu 22.04/24.04.
  Use when the user wants to create, build, test, or analyze a pure C99 project.
license: MIT
compatibility: Requires podman and git. Targets run inside containers.
---

# std-c99-proj — Pure ANSI C99 + Memory Arena Framework

Professional framework for pure ANSI C99 projects. All builds, tests, and
analysis run inside Podman containers for reproducibility. Output is per-target
in `build/<target>/` and `docs/<target>/`.

## Philosophy

- **Pure C99**: `-std=c99 -pedantic` — no extensions, no C11+
- **Memory Arena**: hardened allocator — aligned, overflow-safe, double-init guard
- **No recursion**: iteration only — predictable stack usage
- **Zero tolerance**: `-Werror -Wall -Wextra -Wconversion -Wshadow` — no warnings
- **`-fanalyzer`**: GCC compile-time static analysis (GCC ≥ 10)
- **Valgrind mandatory**: every test run checks for leaks and memory errors
- **Hardened Release**: `-O2 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fPIE`
- **Git mandatory**: every project starts with version control
- **Containerized**: builds never touch the host — fully reproducible
- **AGENTS.md**: generated in every project — AI agents auto-follow rules

## Invoking Scripts

Scripts are in this skill's `scripts/` directory. **Use the full absolute path**
to each script. Do NOT use `SKILL_DIR` as a shell variable — resolve the path
from this SKILL.md's location first.

Example: if this file is at `/home/user/.nvm/.../std-c99-proj/SKILL.md`,
then scripts are at `/home/user/.nvm/.../std-c99-proj/scripts/`.

Always `cd` to the project directory first, then call the script with its
full path.

## Available Actions

### 1. Initialize a project

From an **empty directory** (no `.git`, no `src/`):

```bash
cd /path/to/new-project
/full/path/to/std-c99-proj/scripts/init_project.sh
```

Creates: `AGENTS.md`, `src/`, `include/`, `tests/`, `containers/`,
`CMakeLists.txt`, `.gitignore`, template source files, and a git repository.

**Refuses to run** if the directory already has `.git`, `src/`, `include/`,
`tests/`, or `CMakeLists.txt`.

### 2. Build

```bash
/full/path/to/std-c99-proj/scripts/build.sh <target> [Debug|Release]
```

Targets: `rhel8`, `rhel9`, `rhel10`, `debian11`, `debian12`, `ubuntu2204`, `ubuntu2404`

Output goes to `build/<target>/`. Multiple targets coexist.

### 3. Test with Valgrind

```bash
/full/path/to/std-c99-proj/scripts/test.sh <target>
```

Builds with `BUILD_TESTS=ON` into `build/<target>/`, runs `ctest`, then
Valgrind with `--leak-check=full --error-exitcode=1`. Any leak = hard failure.

### 4. Static analysis

```bash
/full/path/to/std-c99-proj/scripts/static_analysis.sh <target>
```

Runs `clang-tidy` inside the target container with `--warnings-as-errors`.

### 5. Generate documentation

```bash
/full/path/to/std-c99-proj/scripts/docs.sh <target>
```

Doxygen output goes to `docs/<target>/`.

## Target Catalog

| Target      | Family | Base Image        | `-fanalyzer` |
|-------------|--------|-------------------|:------------:|
| `rhel8`     | RHEL   | `rockylinux:8`    | — |
| `rhel9`     | RHEL   | `rockylinux:9`    | ✅ |
| `rhel10`    | RHEL   | `quay.io/rockylinux/rockylinux:10` | ✅ |
| `debian11`  | Debian | `debian:bullseye` | ✅ |
| `debian12`  | Debian | `debian:bookworm` | ✅ |
| `ubuntu2204`| Ubuntu | `ubuntu:22.04`    | ✅ |
| `ubuntu2404`| Ubuntu | `ubuntu:24.04`    | ✅ |

See [references/TARGETS.md](references/TARGETS.md) for full details.

## Compiler Flags

| | Debug | Release |
|---|---|---|
| Optimization | `-O0` | `-O2` |
| Debug info | `-g3` | — |
| Warnings | `-Wall -Wextra -Werror -Wconversion -Wshadow -pedantic` | same |
| Static analysis | `-fanalyzer` (GCC ≥ 10) | — |
| Stack protection | `-fstack-protector-strong` | `-fstack-protector-strong` |
| Buffer overflow | — | `-D_FORTIFY_SOURCE=2` |
| PIE | — | `-fPIE` |

## Framework Rules

1. All code compiles with `-std=c99 -pedantic -Werror` and all warning flags
2. No recursion — use iteration (loops, explicit stacks)
3. Memory via arena only — no direct `malloc`/`free` in application code
4. All tests pass Valgrind with zero leaks and zero errors
5. Git repository required — init creates the first commit
6. All builds inside Podman containers — never on host

See [references/RULES.md](references/RULES.md) for rationale.

## Memory Arena API

```c
int   mem_arena_init(MemArena *arena, size_t size);  // aligned, overflow-safe
void *mem_arena_alloc(MemArena *arena, size_t size);  // aligned to MEM_ARENA_ALIGN
void  mem_arena_reset(MemArena *arena);               // reuse without freeing
void  mem_arena_free(MemArena *arena);                // idempotent
```

See [references/ARENA_API.md](references/ARENA_API.md) for full documentation.

## Project Layout After Init

```
my-project/
├── AGENTS.md
├── CMakeLists.txt
├── Doxyfile
├── .gitignore
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
├── tests/
│   └── test_arena.c
├── build/<target>/          # per-target output
└── docs/<target>/           # per-target docs
```

