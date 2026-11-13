---
name: Add chip backend
description: Scaffold and wire a new emulator chip backend through EmulatorChips, get_chipemu, CLI help text, and the fuzz dictionary; use for H1C3M, H1D3C, H1D3M, NPCX, or other chip support in better-gscemu.
---

# Add chip backend

Only H1B3C is implemented. The help text lists future chips, but they have no backend until you add one.

## Read first

- `AGENTS.md`
- `src/H1B3C/AGENTS.md`
- `include/common/emulator.h`
- `include/H1B3C/emulator.h`
- `src/H1B3C/emulator.c`
- `src/main.c`

## Lifecycle contract

- `initialize(void)` must set `ctx.chipemu.ready = true` only after successful setup. Leave it false on failure.
- `start_emulation(void)` runs after a successful initialize.
- `end_emulation(void)` takes no arguments, uses `ctx` for state, and must tolerate a partially initialized backend.
- `emu_exit` is `_Noreturn`, takes an `Rc`, and may call `end_emulation` before destroying `ctx.ac`. Do not call `exit` from a backend; use an `RC_*` value from `common/rc.h` for fatal errors.

## Steps

1. Add the enum value in `include/common/emulator.h` before `EMULATORCHIPS_END`.
2. Add `include/<CHIP>/emulator.h` with an include guard and the three lifecycle prototypes.
3. Add `src/<CHIP>/emulator.c`. H1B3C uses the `Havn_` prefix; pick a short, consistent prefix for the new chip.
4. Include the new header in `src/main.c`, add a `case` in `get_chipemu`, and fill `ctx.chipemu` with the three function pointers.
5. Extend chip string parsing in `main.c`, update the `--chip` help text, and keep the default at `h1b3c`.
6. Add the lowercase chip name to `tests/fuzz.dict`.
7. Verify with `make clean && make`, `--chip <name> -v`, an invalid chip, and `make fuzz FUZZ_TIME=1`.

See [checklist](references/checklist.md) for the exact file list.

## Boundaries

- Do not add chip-specific fields to `common/Emulator`. Keep backend state private to `src/<CHIP>/`.
- Do not put chip logic in `src/common/`.
- Do not advertise a chip as working until its backend exists and has been smoke-tested.
- If the backend needs per-instance state, extend the interface deliberately instead of hiding state in globals.
