# include/

Public headers. They define the contract for `src/`.

- Keep implementation details in `.c` files. Do not put struct layouts or state in public headers unless the API needs them.
- Every header needs an include guard. Include the headers it uses directly instead of relying on include order.
- `libargs.h` is the public C API. Keep `ArgCheck` opaque and keep the `extern "C"` guard for C++ consumers.
- Put `ARGCHECK_NORETURN` and printf-format attributes on declarations so every caller sees them.
- `common/ctx.h` declares the global `Ctx ctx` container and `ctx_init`. Keep it a plain data struct; resource ownership stays with the code that created the resource, and `ctx.verbose` is the only verbose switch.
- `common/emulator.h` owns `Emulator` and `EmulatorChips`. Keep them chip-agnostic. Adding a field or changing a function pointer signature requires updating every backend and `get_chipemu`.
- `common/exit.h` exposes `emu_exit`; it must stay `ARGCHECK_NORETURN`.
- `common/rc.h` owns `Rc`. Every exit path uses an `RC_*` value; add a new one instead of reusing an unrelated code.
- Chip headers expose only lifecycle entry points. Chip-specific state and helpers stay in `src/<chip>/`.
- Keep prototypes in sync with definitions. A header change is not complete until all implementations, `main.c`, and the fuzz harness compile.
