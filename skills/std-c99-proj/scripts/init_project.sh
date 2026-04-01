#!/bin/bash
set -euo pipefail

# init_project.sh
# Scaffolds a new pure ANSI C99 project with memory arena in the current directory.
# Must be run from the skill directory context — SKILL_DIR is the parent of scripts/.

SKILL_DIR="$(cd "$(dirname "$0")/.." && pwd)"
TEMPLATES="${SKILL_DIR}/assets/templates"
CONTAINERS="${SKILL_DIR}/assets/containers"

# Guard against overwriting existing projects
for check in src include tests CMakeLists.txt; do
    if [ -e "$check" ]; then
        echo "Error: '$check' already exists. Refusing to overwrite."
        exit 1
    fi
done

echo "==> Initializing pure ANSI C99 project..."

# Create directories
for d in src include tests containers; do
    mkdir -p "$d"
done

# Copy source templates
cp "$TEMPLATES/main.c"       src/
cp "$TEMPLATES/mem_arena.c"  src/
cp "$TEMPLATES/utils.c"      src/
cp "$TEMPLATES/mem_arena.h"  include/
cp "$TEMPLATES/utils.h"      include/
cp "$TEMPLATES/test_arena.c" tests/
cp "$TEMPLATES/CMakeLists.txt" .
cp "$TEMPLATES/Doxyfile"     .
cp "$TEMPLATES/.gitignore"   .

# Copy containerfiles
cp "$CONTAINERS/Containerfile.rhel"   containers/
cp "$CONTAINERS/Containerfile.debian" containers/

# Initialize git
git init
git add .
git commit -m "init: pure ANSI C99 project (memory arena, -Werror, no recursion)"

echo "==> Project initialized. Run: bash SKILL_DIR/scripts/build.sh <target>"
