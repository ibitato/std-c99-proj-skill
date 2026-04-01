<div align="center">

# 🏗️ std-c99-proj-skill

**Professional framework for pure ANSI C99 projects — as a [Pi](https://github.com/badlogic/pi-mono) skill.**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![npm](https://img.shields.io/npm/v/std-c99-proj-skill.svg?color=orange)](https://www.npmjs.com/package/std-c99-proj-skill)
[![Agent Skills](https://img.shields.io/badge/Agent_Skills-standard-green.svg)](https://agentskills.io)
[![C Standard](https://img.shields.io/badge/C-C99-informational.svg)]()
[![Podman](https://img.shields.io/badge/Podman-containerized-purple.svg)](https://podman.io)

Memory arena · Containerized builds · Valgrind mandatory · `-fanalyzer` · Zero warnings · No recursion

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
- [Compiler Flags](#compiler-flags)
- [Framework Rules](#framework-rules)
- [Memory Arena API](#memory-arena-api)
- [Requirements](#requirements)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)

## Overview

`std-c99-proj-skill` is an installable [Pi](https://github.com/badlogic/pi-mono) package that provides a complete, opinionated framework for pure ANSI C99 development. It follows the [Agent Skills](https://agentskills.io) standard and can be invoked manually via `/skill:std-c99-proj` or detected automatically by the agent.

The skill scaffolds projects with a hardened memory arena allocator, strict compiler flags, containerized builds via Podman, mandatory Valgrind analysis, and clang-tidy static analysis — enforcing professional-grade quality from the first commit. An `AGENTS.md` is generated in every project so any AI coding agent automatically follows the framework rules.

## Features

| Feature | Description |
|---------|-------------|
| 🧠 **Memory Arena** | Aligned, hardened allocator — overflow protection, double-init guard, zero leaks |
| 📦 **Containerized Builds** | All compilation inside Podman containers — per-target output in `build/<target>/` |
| 🔒 **Strict Compilation** | `-std=c99 -pedantic -Werror -Wall -Wextra -Wconversion -Wshadow` |
| 🔬 **`-fanalyzer`** | GCC compile-time static analysis (GCC ≥ 10) — catches use-after-free, null deref |
| 🛡️ **Hardened Release** | `-O2 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fPIE` |
| 🔄 **No Recursion** | Iteration only — predictable stack usage, no overflow risk |
| 🔍 **Valgrind Mandatory** | Every test run checks for leaks and memory errors |
| 🧹 **Static Analysis** | clang-tidy with `--warnings-as-errors` |
| 📚 **Doxygen Docs** | API documentation generated inside containers |
| 🧪 **27 Tests Included** | Arena tests with alignment, overflow, and edge case coverage |
| 🤖 **AGENTS.md** | Generated in every project — AI agents auto-follow framework rules |

## Install

```bash
pi install npm:std-c99-proj-skill        # from npm (recommended)
pi install git:github.com/ibitato/std-c99-proj-skill   # from git
```

## Quick Start

```
> /skill:std-c99-proj
> Initialize a new C99 project and build it for RHEL 9
```

The agent will:
1. Run `init_project.sh` to scaffold the project (including `AGENTS.md`)
2. Run `build.sh rhel9 Debug` to compile inside a Rocky Linux 9 container
3. Run `test.sh rhel9` to execute 27 tests under Valgrind

Output goes to `build/rhel9/` — multiple targets coexist without overwriting.

## Actions

| Action | Script | Description |
|--------|--------|-------------|
| **init** | `init_project.sh` | Scaffold project with git, templates, tests, Containerfiles, and `AGENTS.md` |
| **build** | `build.sh <target> [Debug\|Release]` | Compile inside container → `build/<target>/` |
| **test** | `test.sh <target>` | Tests + Valgrind → `build/<target>/` (leaks = hard failure) |
| **static-analysis** | `static_analysis.sh <target>` | clang-tidy with `--warnings-as-errors` |
| **docs** | `docs.sh <target>` | Doxygen documentation → `docs/<target>/` |

## Build Targets

All builds run inside Podman containers. Two parametrized Containerfiles cover all targets:

| Target | Family | Base Image | GCC | `-fanalyzer` |
|--------|--------|------------|-----|:------------:|
| `rhel8` | RHEL | `rockylinux:8` | 8.x | — |
| `rhel9` | RHEL | `rockylinux:9` | 11.x | ✅ |
| `rhel10` | RHEL | `quay.io/rockylinux/rockylinux:10` | 14.x | ✅ |
| `debian11` | Debian | `debian:bullseye` | 10.x | ✅ |
| `debian12` | Debian | `debian:bookworm` | 12.x | ✅ |
| `ubuntu2204` | Ubuntu | `ubuntu:22.04` | 11.x | ✅ |
| `ubuntu2404` | Ubuntu | `ubuntu:24.04` | 13.x | ✅ |

Each container includes: `gcc`, `clang`, `clang-tidy`, `cmake`, `make`, `git`, `valgrind`, `doxygen`.

## Project Layout

After running `init`, the generated project has this structure:

```
my-project/
├── AGENTS.md               # AI agent rules (auto-loaded by Pi, Claude Code, etc.)
├── CMakeLists.txt          # C99 strict, Debug/Release flags, BUILD_TESTS option
├── Doxyfile                # Doxygen configuration
├── .gitignore
├── containers/
│   ├── Containerfile.rhel
│   └── Containerfile.debian
├── src/
│   ├── main.c              # Entry point using memory arena
│   ├── mem_arena.c         # Hardened arena allocator
│   └── utils.c             # Utility functions
├── include/
│   ├── mem_arena.h         # Arena API (aligned, overflow-safe)
│   └── utils.h             # Utility headers
├── tests/
│   └── test_arena.c        # 27 assertions, Valgrind-clean
├── build/                  # Per-target output (gitignored)
│   ├── rhel9/
│   └── debian12/
└── docs/                   # Per-target docs (gitignored)
    └── rhel9/
```

## Compiler Flags

| | Debug | Release |
|---|---|---|
| **Optimization** | `-O0` | `-O2` |
| **Debug info** | `-g3` (full + macros) | — |
| **Warnings** | `-Wall -Wextra -Werror -Wconversion -Wshadow -Wfloat-conversion -pedantic` | same |
| **Static analysis** | `-fanalyzer` (GCC ≥ 10) | — |
| **Stack protection** | `-fstack-protector-strong` | `-fstack-protector-strong` |
| **Buffer overflow** | — | `-D_FORTIFY_SOURCE=2` |
| **PIE** | — | `-fPIE` |

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
void *mem_arena_alloc(MemArena *arena, size_t size);   /* aligned, NULL on failure    */
void  mem_arena_reset(MemArena *arena);                /* reuse without freeing       */
void  mem_arena_free(MemArena *arena);                 /* single free, idempotent     */
```

Hardened: aligned allocations, integer overflow protection, double-init guard.
See [`references/ARENA_API.md`](skills/std-c99-proj/references/ARENA_API.md) for full documentation.

## Requirements

- [Podman](https://podman.io/) — container runtime (rootless supported)
- [Git](https://git-scm.com/) — version control
- [Pi](https://github.com/badlogic/pi-mono) — AI coding agent

## Documentation

📖 **[User Manual](MANUAL.md)** — complete guide from installation to advanced usage (beginner / intermediate / advanced).

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) before submitting a pull request.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.
