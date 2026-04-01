# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.1.0] - 2026-04-01

### Added
- Memory arena hardening: aligned allocations (`MEM_ARENA_ALIGN`), integer overflow
  protection, double-init guard
- 11 new test assertions (27 total): alignment, overflow, double-init, alloc-after-free
- Comprehensive user manual (`MANUAL.md`) — beginner / intermediate / advanced
- RHEL 10 support via `quay.io/rockylinux/rockylinux:10`

### Changed
- Containerfile.rhel uses `ARG BASE_IMAGE` (same pattern as Debian) for flexible registries
- EPEL and CRB/PowerTools repos enabled automatically in RHEL containers
- All scripts use unified `BASE_IMAGE` target mapping

### Fixed
- Scripts missing execute permissions
- `doxygen` unavailable in default Rocky Linux repos (now via EPEL)
- `rockylinux:10` not on Docker Hub (now uses quay.io)
- `align_up` wrap-around on near-`SIZE_MAX` allocations
- Unused `align_up` function triggering `-Werror=unused-function`

## [2.0.0] - 2026-04-01

### Added
- `SKILL.md` following the [Agent Skills](https://agentskills.io) standard
- `package.json` with `pi` manifest for `pi install` support
- Parametrized `Containerfile.rhel` (ARG VERSION) for Rocky Linux 8/9/10
- Parametrized `Containerfile.debian` (ARG BASE_IMAGE) for Debian 11/12 and Ubuntu 22.04/24.04
- `build.sh` with target selection and Debug/Release support
- `test.sh` with ctest + Valgrind (`--leak-check=full --error-exitcode=1`)
- `static_analysis.sh` with `compile_commands.json` generation and clang-tidy
- `docs.sh` for Doxygen generation inside containers
- `init_project.sh` with overwrite protection
- Memory arena template (`mem_arena.c/.h`) with Doxygen documentation
- Test template (`test_arena.c`) with 14 assertions
- Reference documentation: `TARGETS.md`, `ARENA_API.md`, `RULES.md`
- `CONTRIBUTING.md` with contribution guidelines
- `LICENSE` (MIT)

### Changed
- Migrated from custom `skill.json` format to Agent Skills standard
- Replaced 3 fixed Podmanfiles with 2 parametrized Containerfiles
- Restructured from flat layout to `skills/std-c99-proj/` package layout

### Removed
- `skill.json` (replaced by `SKILL.md` frontmatter)
- `commands/` directory (replaced by `scripts/`)
- `podmanfiles/` directory (replaced by `assets/containers/`)
- `templates/` directory (replaced by `assets/templates/`)

## [1.4.0] - 2026-04-01

### Added
- Initial prototype with `skill.json` and shell commands
- Podmanfiles for debug, release, and tests
- Basic CMakeLists.txt and source templates
