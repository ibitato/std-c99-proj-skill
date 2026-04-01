#!/bin/bash
set -euo pipefail

# docs.sh <target>
# Generates Doxygen documentation inside the target container.
# Output goes to docs/<target>/ on the host.

TARGET="${1:-}"

if [ -z "$TARGET" ]; then
    echo "Usage: $0 <target>"
    echo "Targets: rhel8 rhel9 rhel10 debian11 debian12 ubuntu2204 ubuntu2404"
    exit 1
fi

case "$TARGET" in
    rhel8)      CFILE="containers/Containerfile.rhel";   BUILD_ARG="BASE_IMAGE=rockylinux:8";;
    rhel9)      CFILE="containers/Containerfile.rhel";   BUILD_ARG="BASE_IMAGE=rockylinux:9";;
    rhel10)     CFILE="containers/Containerfile.rhel";   BUILD_ARG="BASE_IMAGE=quay.io/rockylinux/rockylinux:10";;
    debian11)   CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=debian:bullseye";;
    debian12)   CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=debian:bookworm";;
    ubuntu2204) CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=ubuntu:22.04";;
    ubuntu2404) CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=ubuntu:24.04";;
    *) echo "Error: unknown target '$TARGET'"; exit 1;;
esac

IMAGE_TAG="std-c99-proj:${TARGET}"
DOCS_DIR="docs/${TARGET}"

if ! [ -f "$CFILE" ]; then
    echo "Error: $CFILE not found. Run init first."
    exit 1
fi

echo "==> Building image ${IMAGE_TAG}..."
podman build -t "$IMAGE_TAG" --build-arg "$BUILD_ARG" -f "$CFILE" .

echo "==> Generating documentation -> ${DOCS_DIR}/"
mkdir -p "$DOCS_DIR"
podman run --rm -v "$(pwd):/app:Z" -w /app "$IMAGE_TAG" \
    bash -c "sed 's|^OUTPUT_DIRECTORY.*|OUTPUT_DIRECTORY = ${DOCS_DIR}|' Doxyfile | doxygen -"

echo "==> Documentation generated in ${DOCS_DIR}/"
