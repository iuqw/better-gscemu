# tests/

The libFuzzer harness is the only automated test target.

- `make fuzz` builds `tests/fuzz_libargs.c` with `clang -fsanitize=fuzzer,address,undefined`.
- `FUZZ_TIME` defaults to 60 seconds; `0` runs until interrupted. `FUZZ_ARGS` passes extra libFuzzer flags.
- Committed seeds live in `tests/corpus/`; generated units go to `build/<arch>/fuzz-corpus`.
- Keep `tests/fuzz.dict` current with every flag name, shorthand, and important literal the CLI accepts.
- The harness filters exact `--help` and `-h` tokens because `argcheck_destroy` prints help and exits. Do not remove that filter without isolating help in a child process.
- Consume `argcheck_fequals` and `argcheck_fequals_str` results before `argcheck_destroy`; those pointers are owned by `ArgCheck`.
- Keep the committed corpus small. Add a seed for each new argument syntax: `--flag=value`, space-separated values, and shorthand forms.
- If `libargs` gains another `.c` file, add it to the `$(FUZZ_BIN)` prerequisites in the Makefile.
- Run `make fuzz FUZZ_TIME=1` after parser changes. Any ASan or UBSan report is a bug.
- Do not add network access or launch external programs from the fuzz target.
