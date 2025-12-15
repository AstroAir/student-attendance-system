#!/usr/bin/env bash
set -euo pipefail

echo "cpp-quick-starter setup"

echo "Checking tools..."

for tool in cmake; do
  if ! command -v "$tool" >/dev/null 2>&1; then
    echo "Missing required tool: $tool" >&2
    exit 1
  fi
done

echo "Optional tools: ninja, clang-format, clang-tidy, doxygen, xmake, mkdocs"

echo "Done."
