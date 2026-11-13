# New chip checklist

Copy this list into the task and tick each item.

- [ ] `include/common/emulator.h`: enum value added before `EMULATORCHIPS_END`
- [ ] `include/<CHIP>/emulator.h`: include guard, includes, three prototypes
- [ ] `src/<CHIP>/emulator.c`: all three hooks implemented
- [ ] `initialize` sets `chipemu->ready = true` only after success
- [ ] `end_emulation` is safe after a failed or partial initialize
- [ ] `src/main.c`: header included
- [ ] `src/main.c`: `get_chipemu` case assigns all three pointers
- [ ] `src/main.c`: lowercase chip string maps to the enum
- [ ] `src/main.c`: `--chip` help text updated
- [ ] `tests/fuzz.dict`: lowercase chip string added
- [ ] `make clean && make`
- [ ] `./build/$(uname -m)/gscemu --chip <name> -v`
- [ ] invalid chip still exits non-zero
- [ ] `make fuzz FUZZ_TIME=1`

If the chip is not ready, do not leave a half-wired `case` that advertises support. Keep the help text honest.
