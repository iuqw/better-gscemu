#!/usr/bin/env bash
set -euo pipefail

ROOT="$(git rev-parse --show-toplevel)"
cd "$ROOT"

TMP_O="$(mktemp)"
FUZZ_LOG="$(mktemp)"
trap 'rm -f "$TMP_O" "$FUZZ_LOG"' EXIT

make clean
make

BIN="build/$(uname -m)/gscemu"

"$BIN" --help >/dev/null
"$BIN" >/dev/null
"$BIN" --chip h1b3c -v >/dev/null

if "$BIN" --chip invalid >/dev/null 2>&1; then
  echo "verify-c: invalid chip unexpectedly succeeded" >&2
  exit 1
fi

if command -v clang >/dev/null 2>&1; then
  if ! make fuzz FUZZ_TIME=1 >"$FUZZ_LOG" 2>&1; then
    cat "$FUZZ_LOG" >&2
    exit 1
  fi
else
  echo "verify-c: clang not found, skipped fuzzing" >&2
fi

if command -v gcc >/dev/null 2>&1; then
  gcc -Iinclude -std=c11 -Wall -Wextra -Wformat=2 -fanalyzer \
    -c src/libargs.c -o "$TMP_O"
fi

if command -v clang >/dev/null 2>&1; then
  clang --analyze -Iinclude -std=c11 -Wall -Wextra src/main.c -o /dev/null
fi

echo "verify-c: ok"
