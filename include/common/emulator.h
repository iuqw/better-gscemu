#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>

#include <libargs.h>

typedef struct {
  bool ready;
  void (*initialize)(ArgCheck *ac);
  void (*start_emulation)(void);
  void (*end_emulation)(void);
} Emulator;

typedef enum {
  H1B3C = 0,

  EMULATORCHIPS_END
} EmulatorChips;

#endif
