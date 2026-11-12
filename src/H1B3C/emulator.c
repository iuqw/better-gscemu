#include <libargs.h>
#include <common/logger.h>
#include <H1B3C/emulator.h>

void Havn_Initialize(ArgCheck *ac) {
  LOG("initialized\n");
  (void)ac;
}

void Havn_StartEmulation(void) {
  LOG("started emu\n");
}

void Havn_EndEmulation(void) {
  LOG("ended emu\n");
}
