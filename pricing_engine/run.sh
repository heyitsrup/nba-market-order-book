#!/usr/bin/env bash
set -euo pipefail

# ---- Config ----
BUILD_DIR="build"
BUILD_TYPE="${BUILD_TYPE:-Release}"
JOBS="${JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)}"
SOURCE_EXTS=("*.c" "*.cc" "*.cpp" "*.h" "*.hpp")

# ---- Helpers ----
info() { printf '\033[1;34m[run.sh]\033[0m %s\n' "$1"; }
err()  { printf '\033[1;31m[run.sh]\033[0m %s\n' "$1" >&2; }

# ---- 1. Format ----
format_code() {
    if ! command -v clang-format &>/dev/null; then
        err "clang-format not found, skipping formatting."
        return
    fi

    if [ ! -f .clang-format ]; then
        err ".clang-format not found in project root, skipping formatting."
        return
    fi

    info "Formatting source files with clang-format..."

    local find_args=()
    for ext in "${SOURCE_EXTS[@]}"; do
        find_args+=(-o -name "$ext")
    done
    # drop the leading -o
    find_args=("${find_args[@]:1}")

    find . \( -path "./${BUILD_DIR}" -o -path "./.git" \) -prune -o \
        -type f \( "${find_args[@]}" \) -print0 \
        | xargs -0 --no-run-if-empty clang-format -i

    info "Formatting complete."
}

# ---- 2. Configure (CMake) ----
configure_build() {
    if [ ! -f CMakeLists.txt ]; then
        err "No CMakeLists.txt found in project root. Aborting."
        exit 1
    fi

    info "Configuring CMake (${BUILD_TYPE}) in ${BUILD_DIR}/..."
    cmake -S . -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
}

# ---- 3. Build (make) ----
build_project() {
    info "Building with make (-j${JOBS})..."
    cmake --build "${BUILD_DIR}" -- -j"${JOBS}"
}

# ---- Main ----
main() {
    format_code
    configure_build
    build_project
    info "Done. Build output is in ./${BUILD_DIR}"
    ./build/pricing_engine
}

main "$@"
