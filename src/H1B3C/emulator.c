#include <stdbool.h>

#include <libargs.h>
#include <common/logger.h>
#include <common/emulator.h>
#include <H1B3C/emulator.h>

void Havn_Initialize(Emulator *chipemu, ArgCheck *ac) {
  chipemu->ready = true;
  LOG("initialized\n");
  (void)ac;
}

void Havn_StartEmulation(void) {

  LOG("started emu\n");
}

void Havn_EndEmulation(void) {
  LOG("ended emu\n");
}
