#!/bin/bash
set -euo pipefail

# build.sh <target> [Debug|Release]
# Builds the project inside a Podman container for the given target.

TARGET="${1:-}"
BUILD_TYPE="${2:-Debug}"

usage() {
    echo "Usage: $0 <target> [Debug|Release]"
    echo "Targets: rhel8 rhel9 rhel10 debian11 debian12 ubuntu2204 ubuntu2404"
    exit 1
}

[ -z "$TARGET" ] && usage

# Resolve target to containerfile and base image
case "$TARGET" in
    rhel8)      CFILE="containers/Containerfile.rhel";   BUILD_ARG="BASE_IMAGE=rockylinux:8";;
    rhel9)      CFILE="containers/Containerfile.rhel";   BUILD_ARG="BASE_IMAGE=rockylinux:9";;
    rhel10)     CFILE="containers/Containerfile.rhel";   BUILD_ARG="BASE_IMAGE=quay.io/rockylinux/rockylinux:10";;
    debian11)   CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=debian:bullseye";;
    debian12)   CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=debian:bookworm";;
    ubuntu2204) CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=ubuntu:22.04";;
    ubuntu2404) CFILE="containers/Containerfile.debian"; BUILD_ARG="BASE_IMAGE=ubuntu:24.04";;
    *) echo "Error: unknown target '$TARGET'"; usage;;
esac

IMAGE_TAG="std-c99-proj:${TARGET}"

if ! [ -f "$CFILE" ]; then
    echo "Error: $CFILE not found. Run init first."
    exit 1
fi

echo "==> Building image ${IMAGE_TAG}..."
podman build -t "$IMAGE_TAG" --build-arg "$BUILD_ARG" -f "$CFILE" .

echo "==> Compiling (${BUILD_TYPE}) for ${TARGET}..."
podman run --rm -v "$(pwd):/app:Z" -w /app "$IMAGE_TAG" \
    bash -c "cmake -B build -DCMAKE_BUILD_TYPE=${BUILD_TYPE} && cmake --build build"

echo "==> Build OK: ${TARGET} (${BUILD_TYPE})"
