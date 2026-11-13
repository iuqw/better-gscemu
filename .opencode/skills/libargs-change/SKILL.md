---
name: Change libargs
description: Change the libargs parser or public API safely; use for flags, help output, value parsing, allocation, or ArgCheck lifetime work in better-gscemu.
---

# Change libargs

Read `src/libargs.c`, `include/libargs.h`, `tests/fuzz_libargs.c`, and `tests/AGENTS.md` before editing.

## Invariants

- `ArgCheck` is opaque. Keep the struct definition in `src/libargs.c`.
- `argcheck_new` deep-copies `argv`; `argcheck_destroy` frees the copy.
- `argcheck_fequals` and `argcheck_fequals_str` return pointers into `ArgCheck` storage. They are valid only until `argcheck_destroy`.
- `argcheck_fequals` returns NULL when absent. `argcheck_fequals_str` returns `""`.
- Every lookup registers the flag for help output. Duplicate registrations are ignored.
- `argcheck_check_help` prints help and exits 0 for `-h`/`--help`. `argcheck_destroy` calls it too.
- `argcheck_destroy` is not idempotent. Never call it twice.
- Check every allocation and guard multiplication with `SIZE_MAX` before `malloc`/`realloc`.

## Parsing behavior

Both value and boolean lookups scan `argv` in order. Value lookups accept:

```text
--flag=VALUE
--flag VALUE
-f=VALUE
-f VALUE
```

The prefix check must be followed by an exact length check before `=`. Do not accept `--flaggy=value` as `--flag`.

## Workflow

1. Add or change the prototype in `include/libargs.h` if the public API changes.
2. Implement the behavior in `src/libargs.c` and keep help registration consistent.
3. Update `tests/fuzz_libargs.c` to call the new path.
4. Add flags and literals to `tests/fuzz.dict`.
5. Add a seed under `tests/corpus/` for new syntax.
6. Run `make clean && make` and `make fuzz FUZZ_TIME=1`.

See [API contract](references/api-contract.md) for the detailed invariants.

## Boundaries

- Do not return pointers into freed storage.
- Do not add unbounded allocations or unchecked arithmetic.
- Do not turn help output into a format-string path; descriptions are printed with `%s`.
- Do not weaken the Makefile format warnings to make a change compile.
