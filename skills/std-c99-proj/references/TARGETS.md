# Build Targets

## Target Catalog

| Target       | Family | Base Image          | GCC  | `-fanalyzer` |
|--------------|--------|---------------------|------|:------------:|
| `rhel8`      | RHEL   | `rockylinux:8`      | 8.x  | — |
| `rhel9`      | RHEL   | `rockylinux:9`      | 11.x | ✅ |
| `rhel10`     | RHEL   | `quay.io/rockylinux/rockylinux:10` | 14.x | ✅ |
| `debian11`   | Debian | `debian:bullseye`   | 10.x | ✅ |
| `debian12`   | Debian | `debian:bookworm`   | 12.x | ✅ |
| `ubuntu2204` | Ubuntu | `ubuntu:22.04`      | 11.x | ✅ |
| `ubuntu2404` | Ubuntu | `ubuntu:24.04`      | 13.x | ✅ |

## Containerfiles

Two parametrized Containerfiles cover all targets:

- `containers/Containerfile.rhel` — uses `ARG BASE_IMAGE`
- `containers/Containerfile.debian` — uses `ARG BASE_IMAGE`

## Installed Tools (all targets)

`gcc`, `clang`, `clang-tidy`, `cmake`, `make`, `git`, `valgrind`, `doxygen`.

## Output Directories

Each target gets its own output directory:

```
build/<target>/     # binaries, object files, test binaries
docs/<target>/      # Doxygen HTML
```

Multiple targets coexist without overwriting each other.

## Usage

```bash
bash SKILL_DIR/scripts/build.sh rhel9 Debug       # -> build/rhel9/
bash SKILL_DIR/scripts/build.sh ubuntu2404 Release # -> build/ubuntu2404/
bash SKILL_DIR/scripts/test.sh debian12            # -> build/debian12/
bash SKILL_DIR/scripts/docs.sh rhel9               # -> docs/rhel9/
```
