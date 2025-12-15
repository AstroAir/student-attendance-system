#!/usr/bin/env bash
set -euo pipefail

if ! command -v clang-format >/dev/null 2>&1; then
  echo "clang-format not found" >&2
  exit 1
fi

files=$(git ls-files "*.cpp" "*.hpp")

if [ -z "${files}" ]; then
  exit 0
fi

clang-format -i ${files}
