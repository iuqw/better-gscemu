#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>

#include <libargs.h>

typedef struct Emulator Emulator;

struct Emulator {
  bool ready;
  void (*initialize)(Emulator *chipemu, ArgCheck *ac);
  void (*start_emulation)(void);
  void (*end_emulation)(void);
};

typedef enum {
  H1B3C = 0,

  EMULATORCHIPS_END
} EmulatorChips;

#endif
