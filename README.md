<div align="center">

# 🏗️ std-c99-proj-skill

**Professional framework for pure ANSI C99 projects — as a [Pi](https://github.com/badlogic/pi-mono) skill.**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Pi Package](https://img.shields.io/badge/pi--package-v2.0.0-orange.svg)](https://github.com/ibitato/std-c99-proj-skill)
[![Agent Skills](https://img.shields.io/badge/Agent_Skills-standard-green.svg)](https://agentskills.io)
[![C Standard](https://img.shields.io/badge/C-C99-informational.svg)]()
[![Podman](https://img.shields.io/badge/Podman-containerized-purple.svg)](https://podman.io)

Memory arena · Containerized builds · Valgrind mandatory · Zero warnings · No recursion

---

</div>

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Install](#install)
- [Quick Start](#quick-start)
- [Actions](#actions)
- [Build Targets](#build-targets)
- [Project Layout](#project-layout)
- [Framework Rules](#framework-rules)
- [Memory Arena API](#memory-arena-api)
- [Requirements](#requirements)
- [Contributing](#contributing)
- [License](#license)

## Overview

`std-c99-proj-skill` is an installable [Pi](https://github.com/badlogic/pi-mono) package that provides a complete, opinionated framework for pure ANSI C99 development. It follows the [Agent Skills](https://agentskills.io) standard and can be invoked manually via `/skill:std-c99-proj` or detected automatically by the agent.

The skill scaffolds projects with a memory arena allocator, strict compiler flags, containerized builds via Podman, mandatory Valgrind analysis, and clang-tidy static analysis — enforcing professional-grade quality from the first commit.

## Features

| Feature | Description |
|---------|-------------|
| 🧠 **Memory Arena** | Single-alloc, bulk-free allocator — zero leaks by design |
| 📦 **Containerized Builds** | All compilation runs inside Podman containers — never on host |
| 🔒 **Strict Compilation** | `-std=c99 -pedantic -Werror -Wall -Wextra -Wconversion -Wshadow` |
| 🔄 **No Recursion** | Iteration only — predictable stack usage, no overflow risk |
| 🔍 **Valgrind Mandatory** | Every test run checks for leaks and memory errors |
| 🧹 **Static Analysis** | clang-tidy with `--warnings-as-errors` |
| 📚 **Doxygen Docs** | API documentation generated inside containers |
| 🧪 **Tests Included** | Arena tests with 14 assertions, Valgrind-clean from day zero |

## Install

```bash
pi install git:github.com/ibitato/std-c99-proj-skill
```

## Quick Start

```
> /skill:std-c99-proj
> Initialize a new C99 project and build it for RHEL 9
```

The agent will:
1. Run `init_project.sh` to scaffold the project
2. Run `build.sh rhel9 Debug` to compile inside a Rocky Linux 9 container
3. Run `test.sh rhel9` to execute tests under Valgrind

## Actions

| Action | Script | Description |
|--------|--------|-------------|
| **init** | `init_project.sh` | Scaffold project with git, templates, tests, and Containerfiles |
| **build** | `build.sh <target> [Debug\|Release]` | Compile inside a Podman container for the given target |
| **test** | `test.sh <target>` | Run tests + Valgrind memory analysis (leaks = hard failure) |
| **static-analysis** | `static_analysis.sh <target>` | Run clang-tidy with `--warnings-as-errors` |
| **docs** | `docs.sh <target>` | Generate Doxygen documentation |

## Build Targets

All builds run inside Podman containers. Two parametrized Containerfiles cover all targets:

| Target | Family | Base Image | Containerfile |
|--------|--------|------------|---------------|
| `rhel8` | RHEL | `rockylinux:8` | `Containerfile.rhel` |
| `rhel9` | RHEL | `rockylinux:9` | `Containerfile.rhel` |
| `rhel10` | RHEL | `rockylinux:10` | `Containerfile.rhel` |
| `debian11` | Debian | `debian:bullseye` | `Containerfile.debian` |
| `debian12` | Debian | `debian:bookworm` | `Containerfile.debian` |
| `ubuntu2204` | Ubuntu | `ubuntu:22.04` | `Containerfile.debian` |
| `ubuntu2404` | Ubuntu | `ubuntu:24.04` | `Containerfile.debian` |

Each container image includes: `gcc`, `clang`, `clang-tidy`, `cmake`, `make`, `git`, `valgrind`, `doxygen`.

## Project Layout

After running `init`, the generated project has this structure:

```
my-project/
├── CMakeLists.txt          # C99 strict, BUILD_TESTS option
├── Doxyfile                # Doxygen configuration
├── .gitignore
├── containers/
│   ├── Containerfile.rhel
│   └── Containerfile.debian
├── src/
│   ├── main.c              # Entry point using memory arena
│   ├── mem_arena.c         # Arena allocator implementation
│   └── utils.c             # Utility functions
├── include/
│   ├── mem_arena.h         # Arena API (init, alloc, reset, free)
│   └── utils.h             # Utility headers
└── tests/
    └── test_arena.c        # 14 assertions, Valgrind-clean
```

## Framework Rules

Every project created with this skill enforces these rules as build errors:

1. **Pure C99** — `-std=c99 -pedantic` with all warnings as errors
2. **No recursion** — iteration only, predictable stack usage
3. **Arena-only memory** — no direct `malloc`/`free` in application code
4. **Valgrind clean** — `--leak-check=full --error-exitcode=1` on every test
5. **Git required** — version control from the first commit
6. **Containerized** — all builds inside Podman, never on host

See [`references/RULES.md`](skills/std-c99-proj/references/RULES.md) for rationale and coding conventions.

## Memory Arena API

```c
int   mem_arena_init(MemArena *arena, size_t size);   /* 0 on success, -1 on failure */
void *mem_arena_alloc(MemArena *arena, size_t size);   /* NULL on failure             */
void  mem_arena_reset(MemArena *arena);                /* reuse without freeing       */
void  mem_arena_free(MemArena *arena);                 /* single free, idempotent     */
```

See [`references/ARENA_API.md`](skills/std-c99-proj/references/ARENA_API.md) for full documentation and usage patterns.

## Requirements

- [Podman](https://podman.io/) — container runtime (rootless supported)
- [Git](https://git-scm.com/) — version control
- [Pi](https://github.com/badlogic/pi-mono) — AI coding agent

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) before submitting a pull request.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.
