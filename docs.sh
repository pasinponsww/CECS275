#!/usr/bin/env sh
# Generates Doxygen HTML documentation.

set -eu

mkdir -p docs

doxygen Doxyfile

echo ""
echo "Doxygen HTML generated:"
echo "docs/doxygen/html/index.html"
