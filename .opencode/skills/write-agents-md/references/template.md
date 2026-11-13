# AGENTS.md templates

## Root

````markdown
# AGENTS.md

`better-gscemu` is a standalone C11 reimplementation of GSCEmulator. It parses a small CLI, selects a chip backend, and runs an emulator lifecycle. Only H1B3C is implemented.

## Commands

```bash
make
make clean
make fuzz
```

## Layout

- `src/main.c` - CLI and lifecycle.
- `src/libargs.c`, `include/libargs.h` - parser.
- `src/common/` - shared runtime.
- `src/H1B3C/` - Haven backend.
- `tests/` - fuzz harness.

## Runtime contract

- <CLI behavior>
- <libargs pointer lifetime and help behavior>
- <Emulator lifecycle and `emu_exit` ownership>

## Security rules

- <Untrusted input, format strings, allocation checks>

## Code style

- C11, Google C style, 2-space indent.
- No comments unless genuinely hard to follow.
- <Module boundaries>

## Verification

```bash
make clean && make
make fuzz FUZZ_TIME=1
```

## Working rules

- Read the nearest `AGENTS.md` first.
- Apply changes directly.
- Ask before adding dependencies or destructive behavior.
````

## Nested

```markdown
# <path>

<One-sentence module purpose.>

- <Rule that applies only to this module>
- <Cross-file change requirement>
- <Known gotcha or boundary>

See the root `AGENTS.md` for repo-wide commands and style.
```
