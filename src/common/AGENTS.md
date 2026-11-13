# src/common

Chip-agnostic runtime code shared by every backend.

- `emulator.h` defines the interface every chip implements. Keep it generic; do not add Haven-specific state or fields here.
- `exit.c` owns `emu_exit`. It calls `end_emulation` when given a backend, destroys the `ArgCheck`, then exits. It is `_Noreturn`; add no cleanup after it and never double-destroy `ac`.
- Use `emu_exit` only for unrecoverable setup or runtime failures, not normal control flow.
- `logger.c` owns `LOG`, `LOG_ERR`, and `LOG_VBS`. Every message is a printf format string and must be a compile-time literal; pass data with `%s`, `%d`, and friends.
- `LOG_ERR` currently writes to stdout. Do not assume stderr unless you change the logger and all callers together.
- The log macros use the GNU `##__VA_ARGS__` extension. GCC and Clang are supported; do not assume MSVC.
- Do not include chip headers from this directory. If a common interface grows chip-specific behavior, move that behavior into the backend instead.
- Changing an `Emulator` function pointer signature is a cross-cutting change: update the header, every backend, and `get_chipemu` in `main.c` in one commit.
