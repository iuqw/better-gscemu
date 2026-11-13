# libargs API contract

## Ownership

`argcheck_new(argc, argv)` copies `argv`. The caller keeps ownership of the original array. `argcheck_destroy` frees every string, the flag metadata, the description, the program name, and the `ArgCheck` itself.

Pointers returned by `argcheck_fequals` and `argcheck_fequals_str` point into the copied argv. They are valid until `argcheck_destroy`.

## Help

`argcheck_check_help` exits the process when `help_requested` is set. `argcheck_destroy` calls it before freeing. `argcheck_show_help` always exits.

`-h` and `--help` are reserved. A registered flag named `-h` will not override the built-in help behavior.

## Registration

Every `argcheck_fequals`, `argcheck_fequals_str`, and `argcheck_fbool` call registers the flag's long name, short name, and description. Re-registering the same long name is a no-op, so the first description wins.

Help output sorts by long name. Missing short names render as four spaces, matching the width of `-X, `.

## Allocation and overflow

`register_flag` grows the flag array by doubling. It refuses to grow past `INT_MAX / 2`, and refuses a `new_cap` whose `new_cap * sizeof(FlagInfo)` would overflow `size_t`.

The help printer allocates an array of `FlagInfo *` and guards its multiplication with `SIZE_MAX`. Keep that guard if you touch the printer.

## Fuzzing

The harness filters exact `--help` and `-h` tokens because destroying an `ArgCheck` with help requested exits the process. Any new lookup path should be exercised in `tests/fuzz_libargs.c`.
