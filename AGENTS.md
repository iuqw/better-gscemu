# AGENTS.md

`better-gscemu` is a standalone C11 reimplementation of GSCEmulator. It parses a small CLI, selects a chip backend, and runs an emulator lifecycle. Only H1B3C has a backend today; `h1c3m`, `h1d3c`, `h1d3m`, and `npcx` are placeholders in the help text.

## Commands

```bash
make                         # build build/$(uname -m)/gscemu
make clean                   # remove the current architecture's build dir
make fuzz                    # clang/libFuzzer + ASan/UBSan, 60s default
make fuzz FUZZ_TIME=300      # longer run
make fuzz FUZZ_TIME=0        # run until interrupted
make fuzz FUZZ_ARGS='-runs=1000000'
```

Run the binary directly:

```bash
./build/$(uname -m)/gscemu
./build/$(uname -m)/gscemu --help
./build/$(uname -m)/gscemu --chip h1b3c -v
```

`make run` currently has an absolute-path bug; use the built binary until the target is fixed.

- Normal builds use `gcc`; fuzzing uses `clang`.
- A normal build must stay warning-free. Do not add `-w` or disable format warnings.
- There is no unit-test suite yet. Fuzzing plus a manual CLI smoke run is the current check.

## Layout

- `src/main.c` - CLI flow, chip selection, and emulator lifecycle.
- `src/libargs.c`, `include/libargs.h` - argv parser and help printer.
- `src/common/` - chip-agnostic emulator interface, exit handling, logger.
- `src/H1B3C/` - Haven chip backend (H1B3C).
- `tests/` - libFuzzer harness, dictionary, and seed corpus.
- `include/` - public headers; implementation stays in `src/`.
- `.opencode/skills/` - repo-local skills for fuzzing, chip work, libargs, and verification.

Examples belong in `examples/`. Reference material belongs in `reference/`, which is ignored. `flash/` and `build/` are generated or local-only; never edit or commit them.

## Runtime contract

CLI:

- `--chip` / `-c` selects a chip. Omitting it means `h1b3c`.
- `--verbose` / `-v` enables verbose logging.
- Unknown chips exit with failure. Do not silently map them to H1B3C.
- `-h` / `--help` prints help and exits 0 through `argcheck_check_help` or `argcheck_destroy`.

libargs:

- `argcheck_new` deep-copies `argv`; `argcheck_destroy` frees everything.
- `argcheck_fequals` and `argcheck_fequals_str` return pointers into `ArgCheck` storage. They are valid only until `argcheck_destroy`. `argcheck_fequals` returns NULL when absent; `argcheck_fequals_str` returns `""`.
- `argcheck_fbool` returns 0 or 1.
- Every lookup also registers the flag for help output. Keep long names, short names, and descriptions stable once used.
- `argcheck_destroy` is not idempotent. Do not call it twice.

Emulator:

- `get_chipemu(chip)` fills `ctx.chipemu` with `initialize`, `start_emulation`, and `end_emulation`.
- `initialize()` must set `ctx.chipemu.ready = true` only after successful setup. `main` exits when `ready` is false.
- `start_emulation()` and `end_emulation()` take no arguments and use `ctx`. `end_emulation` must tolerate a partially initialized backend because `emu_exit` may call it on failure.
- `emu_exit(Rc rc)` is `_Noreturn`: it calls `end_emulation` when one is set, destroys `ctx.ac`, and exits with `rc`. It clears `ctx.chipemu.end_emulation` and `ctx.ac` before running them, so a nested `emu_exit` cannot double-close or double-free. Never call `exit()` directly.

Context:

- `common/ctx.h` exposes one global `Ctx ctx` holding `ac`, `chip`, `chipemu`, `uc`, and `verbose`.
- `ctx_init` zeroes the container; `main` calls it before touching anything else.
- `ctx.verbose` is the only verbose switch. `LOG_VBS` reads it; do not add a second flag.
- Ownership follows the field: `argcheck_destroy` frees `ctx.ac`, and a backend's `end_emulation` closes `ctx.uc` and sets it to NULL. Never destroy or close the same resource twice.

Return codes:

- `common/rc.h` defines `Rc`. Every `emu_exit` call, `exit` call, and `main` return uses an `RC_*` value; never pass a raw number.
- `RC_SUCCESS` covers a normal run and the help path.
- `RC_ARGCHECK_FAILED`, `RC_ARG_CHIP_INVALID`, `RC_CHIPEMU_FAILED_INIT`, `RC_CHIPEMU_UNSUPPORTED`, and `RC_LOGGER_INVALID_TYPE` cover failures.
- Values are negative, so the process status is the low byte (`RC_ARG_CHIP_INVALID` reports as 254). Add a new `RC_*` value instead of reusing an unrelated one.

## Security rules

- Treat ROMs, CLI arguments, paths, and guest data as untrusted.
- Never pass untrusted data as a `printf`/`LOG*` format string. Use `LOG("%s", value)`. The Makefile makes non-literal formats a build error.
- Check every allocation. Use `size_t` and overflow guards for allocation arithmetic.
- Keep the public API opaque. Do not expose internal structs in headers.
- Do not add network access, host device I/O, or destructive operations without explicit approval.
- Do not claim a chip is implemented until it has a backend wired through `get_chipemu` and a smoke-tested boot path.

## Code style

- C11, Google C style, 2-space indentation.
- Opening brace on the same line. Keep function signatures on one line when practical; do not split parameters just to fit a column limit.
- No comments unless the code is genuinely hard to follow. Use `/* ... */`, never `//`.
- Include order: own header first where applicable, then standard headers, then project headers, with blank lines between groups.
- Source files use `"header.h"` for their own header and `<module/header.h>` for project headers.
- Headers use include guards, include what they use, and keep C++ guards only where the public C API needs them.
- Prefer `snprintf` over `sprintf` or `strcpy`. Check truncation where the result matters.
- Keep chip-specific logic in the chip backend. `src/common/` must not include `H1B3C` or future chip headers.
- When an interface changes, update the header, every implementation, `main.c`, and the fuzz harness in the same change.
- New `src/**/*.c` files are picked up automatically by the Makefile.

### Makefile

- Recipe lines use tabs; variable assignments and indentation use 2 spaces.
- Keep hardening flags and format warnings. Do not disable them to make a build pass.
- Put build products under `build/<arch>/`. `make clean` only removes the current architecture.
- Add fuzz paths through the `FUZZ_*` variables instead of hardcoding them.

## Verification

```bash
make clean && make
./build/$(uname -m)/gscemu --help
./build/$(uname -m)/gscemu --chip h1b3c -v
make fuzz FUZZ_TIME=1
```

Run the fuzz target after touching `libargs`, CLI parsing, or the fuzz harness.

Optional static checks:

```bash
gcc -Iinclude -std=c11 -Wall -Wextra -Wformat=2 -fanalyzer -c src/libargs.c -o /tmp/libargs.o
clang --analyze -Iinclude -std=c11 -Wall -Wextra src/main.c -o /dev/null
```

A normal build must produce no warnings. Sanitizer or fuzzer findings are bugs; fix them instead of suppressing them.

## Commit messages

History uses signed commits and a short prefix:

```text
feat: add h1c3m chip skeleton
fix: guard libargs flag growth overflow
test: extend fuzzer for --chip
docs: document emulator lifecycle
```

- Lowercase imperative summary, 45-67 characters, no trailing period.
- Add a body when the reason or behavior change is not obvious.
- Always add `Signed-off-by: kxtzownsu <kxtzownsu@gmail.com>`.
- No emoji, `WIP`, bracketed prefixes, vague summaries, or AI/tooling attribution.
- Do not rewrite unrelated history or squash commits unless asked.

## Working rules

- Read the current files and `git log` before changing behavior.
- Apply changes directly to files; do not just describe them.
- Preserve user edits and avoid unrelated cleanup.
- Follow the nearest `AGENTS.md` for the directory you are editing.
- Keep `main.c` small: wiring and lifecycle only; chip work belongs in its backend.
- If a task changes the build, security-sensitive parsing, or a public interface, run the checks above and report the commands run.
- Ask before adding dependencies, network access, or destructive hardware behavior.
