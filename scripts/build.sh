#!/usr/bin/env bash
set -euo pipefail

preset="${1:-ninja-debug}"

cmake --preset "$preset"
cmake --build --preset "$preset"
