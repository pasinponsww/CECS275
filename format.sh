#!/usr/bin/env sh
# Checks or applies clang-format to C++ source files.

set -eu

FILES=$(find common gui/src \( -iname '*.h' -o -iname '*.cpp' \))

if [ "${1:-}" = "--fix" ]; then
    clang-format -i $FILES
    echo "Formatting applied."
else
    clang-format --dry-run --Werror $FILES
    echo "Format check passed."
fi
