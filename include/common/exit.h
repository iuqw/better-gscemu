#ifndef EXIT_H
#define EXIT_H

#include <stdint.h>

#include <libargs.h>
#include <common/emulator.h>

ARGCHECK_NORETURN void emu_exit(uint32_t rc, ArgCheck *ac, Emulator *chipemu);

#endif /* EXIT_H */