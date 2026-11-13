# src/H1B3C

H1B3C is the Haven chip backend. Its entry points use the `Havn_` prefix.

- Public hooks are declared in `include/H1B3C/emulator.h` and wired through `get_chipemu` in `src/main.c`.
- `Havn_Initialize(Emulator *chipemu, ArgCheck *ac)` must set `chipemu->ready = true` only after setup succeeds. `main` exits through `emu_exit` when `ready` is false.
- The current stub only logs and never sets `ready`. Treat that as unfinished backend work, not as intended behavior.
- `Havn_StartEmulation` and `Havn_EndEmulation` must not call `exit`. Return normally, or use `emu_exit` for fatal errors.
- `Havn_EndEmulation` must tolerate a partially initialized backend because failure paths call it through `emu_exit`.
- Keep Haven state, register definitions, and chip-specific parsing in this module. Do not add them to `common/Emulator`.
- Use `LOG`, `LOG_VBS`, and `LOG_ERR`; do not call `printf` directly.
- New chips follow the same shape:
  1. add an `EmulatorChips` value in `include/common/emulator.h`
  2. add `include/<CHIP>/emulator.h` and `src/<CHIP>/emulator.c`
  3. add a `case` in `get_chipemu`
  4. update the `--chip` help text, default selection if needed, and `tests/fuzz.dict`
- Do not advertise `h1c3m`, `h1d3c`, `h1d3m`, or `npcx` as working until their backends exist and have been smoke-tested.
