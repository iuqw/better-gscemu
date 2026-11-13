#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdbool.h>

typedef struct Emulator Emulator;

struct Emulator {
  bool ready;
  void (*initialize)(void);
  void (*start_emulation)(void);
  void (*end_emulation)(void);
};

typedef enum {
  H1B3C = 0,

  EMULATORCHIPS_END
} EmulatorChips;

#endif
