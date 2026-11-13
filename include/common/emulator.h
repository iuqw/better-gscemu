#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>
#include <unicorn/unicorn.h>

#include <libargs.h>

typedef struct Emulator Emulator;

struct Emulator {
  bool ready;
  uc_engine *(*initialize)(Emulator *chipemu, ArgCheck *ac);
  void (*start_emulation)(uc_engine *uc);
  void (*end_emulation)(uc_engine *uc);
};

typedef enum {
  H1B3C = 0,

  EMULATORCHIPS_END
} EmulatorChips;

#endif
