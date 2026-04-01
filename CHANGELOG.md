# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.4.0] - 2026-04-01

### Added
- `-fanalyzer` for GCC ≥ 10 in Debug builds (compile-time static analysis)
- Hardened Release flags: `-O2 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fPIE`
- Debug flags: `-O0 -g3` (full debug info with macros)

### Changed
- CMakeLists.txt: separate Debug/Release flag sets with conditional `-fanalyzer`

## [2.3.0] - 2026-04-01

### Changed
- All output is now per-target: `build/<target>/` and `docs/<target>/`
- Multiple targets coexist without overwriting each other
- Binary path printed after build

## [2.2.0] - 2026-04-01

### Added
- `AGENTS.md` template generated in every project
- AI agents (Pi, Claude Code, Cursor) auto-load framework rules when opening a project
- Published to npm as `std-c99-proj-skill`

## [2.1.0] - 2026-04-01

### Added
- Memory arena hardening: aligned allocations (`MEM_ARENA_ALIGN`), integer overflow
  protection, double-init guard
- 11 new test assertions (27 total): alignment, overflow, double-init, alloc-after-free
- Comprehensive user manual (`MANUAL.md`) — beginner / intermediate / advanced
- RHEL 10 support via `quay.io/rockylinux/rockylinux:10`

### Changed
- Containerfile.rhel uses `ARG BASE_IMAGE` for flexible registries
- EPEL and CRB/PowerTools repos enabled automatically in RHEL containers

### Fixed
- Scripts missing execute permissions
- `doxygen` unavailable in default Rocky Linux repos (now via EPEL)
- `rockylinux:10` not on Docker Hub (now uses quay.io)
- `align_up` wrap-around on near-`SIZE_MAX` allocations

## [2.0.0] - 2026-04-01

### Added
- `SKILL.md` following the [Agent Skills](https://agentskills.io) standard
- `package.json` with `pi` manifest for `pi install` support
- Parametrized Containerfiles for RHEL and Debian families
- Scripts: `build.sh`, `test.sh`, `static_analysis.sh`, `docs.sh`, `init_project.sh`
- Memory arena template with Doxygen documentation
- Test template with 14 assertions
- Reference documentation: `TARGETS.md`, `ARENA_API.md`, `RULES.md`
- `CONTRIBUTING.md`, `LICENSE` (MIT)

### Changed
- Migrated from custom `skill.json` to Agent Skills standard
- Restructured to `skills/std-c99-proj/` package layout

### Removed
- `skill.json`, `commands/`, `podmanfiles/`, `templates/` (old structure)
