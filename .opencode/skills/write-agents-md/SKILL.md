---
name: Write AGENTS.md
description: Write or update AGENTS.md files for better-gscemu; use when adding root or nested agent instructions, project commands, conventions, boundaries, or module-specific guidance.
---

# Write AGENTS.md

This repo uses layered `AGENTS.md` files. The closest file to the edited path wins, and the root file still applies. Keep each level focused on what is local to it.

## Root file

`AGENTS.md` should stay short and cover only what applies everywhere:

- One-sentence project description and current status.
- Exact commands: build, clean, run, fuzz, verify.
- Module map, not an exhaustive file tree.
- Runtime contracts: CLI, `libargs`, `Emulator` lifecycle.
- Security rules: untrusted formats, allocation checks, opaque APIs, no host I/O.
- C and Makefile style.
- Verification commands and the commit-message format.
- Working rules and boundaries.

Keep the root under about 150 lines. Link to nested files instead of repeating module rules.

## Nested files

Current modules:

- `include/AGENTS.md` - public headers, opaque APIs, prototype sync.
- `src/common/AGENTS.md` - `emu_exit`, logger, chip-agnostic interface.
- `src/H1B3C/AGENTS.md` - Haven backend, `ready` contract, new-chip steps.
- `tests/AGENTS.md` - fuzz harness, corpus, dictionary, help filtering.

A nested file should contain only module-specific rules and gotchas. Do not copy the root code style or commit rules into every module.

## Project-specific facts to preserve

- `argcheck_fequals` and `argcheck_fequals_str` return pointers valid only until `argcheck_destroy`.
- `argcheck_destroy` is not idempotent and prints help/exits on `-h`/`--help`.
- `emu_exit` is `_Noreturn` and destroys the `ArgCheck`.
- `initialize` must set `chipemu->ready = true` after successful setup.
- `LOG*` messages must be compile-time literals.
- The fuzz harness filters `--help` and `-h` because destroy exits.
- `make run` currently fails because it prepends `./` to an absolute `TARGET`.
- Only H1B3C is implemented; other chip names are placeholders.

## Workflow

1. Read the root file and the nearest nested file before editing either.
2. Verify every command you add actually works in the current tree.
3. Put cross-cutting rules at the root and module rules in the nearest directory.
4. Remove stale paths, commands, and claims instead of leaving them.
5. Check for contradictions between root and nested files.
6. Do not document generated or ignored paths as source.

See [template](references/template.md) for root and nested skeletons.
