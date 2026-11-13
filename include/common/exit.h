#ifndef EXIT_H
#define EXIT_H

#include <stdint.h>

#include <libargs.h>
#include <common/emulator.h>

ARGCHECK_NORETURN void emu_exit(uint32_t rc, ArgCheck *ac, Emulator *chipemu, uc_engine *uc);

#endif /* EXIT_H */