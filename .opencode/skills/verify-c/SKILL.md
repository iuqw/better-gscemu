---
name: Verify C changes
description: Verify better-gscemu C changes before finishing; use to run a clean warning-free build, CLI smoke tests, static analyzers, and a short libFuzzer run.
---

# Verify C changes

Run the bundled script:

```bash
.opencode/skills/verify-c/scripts/verify.sh
```

It checks:

- `make clean && make`
- `--help`, the default run, and `--chip h1b3c -v`
- an invalid chip exits non-zero
- `make fuzz FUZZ_TIME=1` when clang is available
- `gcc -fanalyzer` on `src/libargs.c` and `clang --analyze` on `src/main.c`

## Manual equivalent

```bash
make clean && make
BIN="build/$(uname -m)/gscemu"
"$BIN" --help
"$BIN"
"$BIN" --chip h1b3c -v
"$BIN" --chip invalid || true
make fuzz FUZZ_TIME=1
```

## Rules

- Do not use `make run`; it currently prepends `./` to an absolute `TARGET` and fails.
- A normal build must be warning-free. Do not suppress warnings to get a pass.
- If clang is missing, report that fuzzing was skipped instead of silently passing.
- Sanitizer or analyzer findings are bugs. Fix them or report them with the command and output.

See [checks](references/checks.md) for expected results and optional longer runs.
