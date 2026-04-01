# Contributing to std-c99-proj-skill

Thank you for your interest in contributing. This document provides guidelines
for contributing to this project.

## How to Contribute

1. **Fork** the repository
2. **Create a branch** from `main` for your changes
3. **Make your changes** following the guidelines below
4. **Test** your changes (see [Testing](#testing))
5. **Submit a pull request** with a clear description

## Guidelines

### Code Style

- All C code must be pure ANSI C99 (`-std=c99 -pedantic`)
- Zero warnings with `-Wall -Wextra -Werror -Wconversion -Wshadow`
- No recursion — iteration only
- All public functions documented with Doxygen comments
- Header guards: `#ifndef FILENAME_H` / `#define FILENAME_H`

### Shell Scripts

- Use `#!/bin/bash` with `set -euo pipefail`
- Validate all inputs before use
- Provide clear error messages on failure
- Follow the existing target resolution pattern

### Containerfiles

- Minimize layers
- Clean package manager caches in the same `RUN` layer
- Use `ARG` for parametrization, not hardcoded values

### Documentation

- Keep `SKILL.md` under 500 lines (progressive disclosure)
- Detailed docs go in `references/`
- Use clear, concise English

## Testing

Before submitting, verify that:

1. Templates compile cleanly on at least one RHEL and one Debian target
2. All tests pass under Valgrind with zero leaks
3. clang-tidy reports no issues
4. Shell scripts pass `shellcheck` (if available)

## Reporting Issues

Use [GitHub Issues](https://github.com/ibitato/std-c99-proj-skill/issues) with
the provided templates. Include:

- Target(s) affected
- Steps to reproduce
- Expected vs actual behavior
- Podman version (`podman --version`)

## License

By contributing, you agree that your contributions will be licensed under the
MIT License.
