# std-c99-proj-skill

Pi package providing a professional framework for pure ANSI C99 projects.

## Features

- **Memory Arena** — single-alloc, bulk-free, zero leaks by design
- **Containerized builds** — Podman targets for RHEL 8/9/10, Debian 11/12, Ubuntu 22.04/24.04
- **Strict compilation** — `-std=c99 -pedantic -Werror` with all warnings enabled
- **No recursion** — iteration only, predictable stack usage
- **Valgrind mandatory** — every test run checks for leaks and memory errors
- **Static analysis** — clang-tidy with warnings-as-errors
- **Doxygen docs** — generated inside containers

## Install

```bash
pi install git:github.com/ibitato/std-c99-proj-skill
```

## Usage

Inside pi, invoke the skill:

```
/skill:std-c99-proj
```

Or let the agent detect it automatically when you work on C99 projects.

### Actions

| Action           | What it does                                      |
|------------------|---------------------------------------------------|
| **init**         | Scaffold a new project with git, templates, tests |
| **build**        | Compile for a target (Debug/Release)              |
| **test**         | Run tests + Valgrind memory analysis              |
| **static-analysis** | Run clang-tidy inside container               |
| **docs**         | Generate Doxygen documentation                    |

### Targets

`rhel8` · `rhel9` · `rhel10` · `debian11` · `debian12` · `ubuntu2204` · `ubuntu2404`

## Requirements

- [Podman](https://podman.io/)
- [Git](https://git-scm.com/)

## License

MIT
