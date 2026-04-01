# Build Targets

## Target Catalog

| Target       | Family | Base Image          | Package Manager | Notes                          |
|--------------|--------|---------------------|-----------------|--------------------------------|
| `rhel8`      | RHEL   | `rockylinux:8`      | `dnf`           | RHEL 8 / Alma 8 / Rocky 8     |
| `rhel9`      | RHEL   | `rockylinux:9`      | `dnf`           | RHEL 9 / Alma 9 / Rocky 9     |
| `rhel10`     | RHEL   | `rockylinux:10`     | `dnf`           | RHEL 10 / Alma 10 / Rocky 10  |
| `debian11`   | Debian | `debian:bullseye`   | `apt-get`       | Debian 11 Bullseye             |
| `debian12`   | Debian | `debian:bookworm`   | `apt-get`       | Debian 12 Bookworm             |
| `ubuntu2204` | Debian | `ubuntu:22.04`      | `apt-get`       | Ubuntu 22.04 LTS Jammy         |
| `ubuntu2404` | Debian | `ubuntu:24.04`      | `apt-get`       | Ubuntu 24.04 LTS Noble         |

## Containerfiles

Two parametrized Containerfiles cover all targets:

- `containers/Containerfile.rhel` — uses `ARG VERSION` (8, 9, or 10)
- `containers/Containerfile.debian` — uses `ARG BASE_IMAGE` (debian:X or ubuntu:X)

## Installed Tools (all targets)

| Tool          | Purpose                        |
|---------------|--------------------------------|
| `gcc`         | Primary C compiler             |
| `clang`       | Secondary compiler + tooling   |
| `clang-tidy`  | Static analysis                |
| `cmake`       | Build system                   |
| `make`        | Build backend                  |
| `git`         | Version control                |
| `valgrind`    | Memory error detection         |
| `doxygen`     | Documentation generation       |

## Usage

```bash
# Build for a specific target
bash SKILL_DIR/scripts/build.sh rhel9 Debug
bash SKILL_DIR/scripts/build.sh ubuntu2404 Release

# Test on a specific target
bash SKILL_DIR/scripts/test.sh debian12

# Static analysis on a specific target
bash SKILL_DIR/scripts/static_analysis.sh rhel9
```

Images are cached locally by Podman. First build pulls the base image and
installs packages; subsequent builds reuse the cached image.
