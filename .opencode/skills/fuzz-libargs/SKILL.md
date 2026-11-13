---
name: Fuzz libargs
description: Run, extend, and triage the libFuzzer harness for libargs; use for parser or CLI fuzzing, sanitizer crashes, and corpus or dictionary updates in better-gscemu.
---

# Fuzz libargs

The target is `tests/fuzz_libargs.c` linked with `src/libargs.c`. `make fuzz` builds it with clang, libFuzzer, ASan, and UBSan.

## Run

```bash
make fuzz                     # 60s
make fuzz FUZZ_TIME=300       # 5 min
make fuzz FUZZ_TIME=0         # until ctrl-c
make fuzz FUZZ_ARGS='-runs=1000000'
```

- Seeds: `tests/corpus/`
- Dictionary: `tests/fuzz.dict`
- Generated corpus: `build/$(uname -m)/fuzz-corpus/`

## Change workflow

1. Read `tests/AGENTS.md`, `src/libargs.c`, and `include/libargs.h`.
2. Update `tests/fuzz_libargs.c` when the parser API or call pattern changes. Use returned pointers before `argcheck_destroy`.
3. Add long names, shorthands, and important literals to `tests/fuzz.dict`.
4. Add a small seed under `tests/corpus/` for new syntax.
5. Run `make fuzz FUZZ_TIME=1`, then a longer run for parser rewrites.

## Crash workflow

1. Reproduce the artifact directly against `build/$(uname -m)/fuzz_libargs`.
2. Read the ASan/UBSan report and find the first project frame.
3. Minimize with libFuzzer's `-minimize_crash=1`.
4. Fix the root cause; do not suppress the sanitizer.
5. Add the minimized input as a regression seed and rerun.

See [crash triage](references/crash-triage.md) for the detailed loop.

## Rules

- Keep the exact `--help` and `-h` filter. `argcheck_destroy` prints help and exits, which kills the fuzzer.
- Keep `tests/corpus/` small and focused.
- Do not add network access or external process execution to the harness.
- If `libargs` gains a new `.c` dependency, add it to the `$(FUZZ_BIN)` prerequisites in the Makefile.
