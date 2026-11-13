# Crash triage

## Reproduce

Artifacts are written as `crash-*`, `leak-*`, `timeout-*`, or `oom-*` files.

```bash
BIN="build/$(uname -m)/fuzz_libargs"
"$BIN" path/to/crash-...
```

Use the same sanitizer build. Do not rebuild with `-O2` only; the report is the useful part.

## Minimize

```bash
"$BIN" -minimize_crash=1 path/to/crash-...
```

Keep the minimized file. Confirm it still crashes before the fix and no longer crashes after it.

## Common findings

- ASan heap-buffer-overflow: check size math, loop bounds, and the extra byte for NUL.
- ASan use-after-free: check `ArgCheck` ownership and pointer lifetime.
- UBSan: check signed overflow, shifts, invalid enum values, and null dereference.
- LeakSanitizer: check every error path in `argcheck_new` and `register_flag`.

## Regress

Add the minimized input to `tests/corpus/`, and update `tests/fuzz.dict` when a new token is needed. Run:

```bash
make fuzz FUZZ_TIME=10
```
