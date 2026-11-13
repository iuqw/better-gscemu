# Checks

## Expected results

- `make clean && make` exits 0 with no warnings.
- `./build/$(uname -m)/gscemu --help` exits 0 and prints usage plus registered flags.
- `./build/$(uname -m)/gscemu` exits 0 and runs the H1B3C lifecycle.
- `./build/$(uname -m)/gscemu --chip h1b3c -v` exits 0 and prints verbose logs.
- `./build/$(uname -m)/gscemu --chip invalid` exits with `RC_ARG_CHIP_INVALID` (254).
- `make fuzz FUZZ_TIME=1` exits 0 with no ASan/UBSan report.

## Longer runs

```bash
make fuzz FUZZ_TIME=60
make fuzz FUZZ_ARGS='-runs=1000000'
make fuzz FUZZ_TIME=0
```

Use a longer run after parser rewrites, allocation changes, or new CLI syntax.

## Static analysis

```bash
gcc -Iinclude -std=c11 -Wall -Wextra -Wformat=2 -fanalyzer -c src/libargs.c -o /tmp/libargs.o
clang --analyze -Iinclude -std=c11 -Wall -Wextra src/main.c -o /dev/null
```

Treat analyzer warnings as real until proven otherwise. Do not add blanket suppressions.
